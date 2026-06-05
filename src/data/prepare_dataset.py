from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path

import numpy as np
from sklearn.model_selection import train_test_split

from src.common.config import Config, load_config
from src.data.audio_files import iter_audio_files
from src.features.librosa_mfcc import extract_mfcc, iter_fixed_clips, load_audio


@dataclass(frozen=True)
class LabeledFile:
    path: Path
    label: int
    class_name: str


@dataclass(frozen=True)
class PreparedSplit:
    features: np.ndarray
    labels: np.ndarray
    source_paths: np.ndarray
    clip_start_seconds: np.ndarray
    clip_rms: np.ndarray


def split_class_files(
    files: list[LabeledFile],
    config: Config,
) -> tuple[list[LabeledFile], list[LabeledFile], list[LabeledFile]]:
    train_val, test = train_test_split(
        files,
        test_size=config.test_size,
        random_state=config.random_seed,
        shuffle=True,
    )
    train, val = train_test_split(
        train_val,
        test_size=config.validation_size,
        random_state=config.random_seed,
        shuffle=True,
    )
    return list(train), list(val), list(test)


def collect_labeled_files(config: Config) -> list[LabeledFile]:
    labeled_files: list[LabeledFile] = []
    for label, class_name in enumerate(config.classes):
        class_files = iter_audio_files(config.data_dir / class_name)
        if not class_files:
            raise RuntimeError(f"No audio files found for class: {class_name}")
        labeled_files.extend(
            LabeledFile(path=path, label=label, class_name=class_name)
            for path in class_files
        )
    return labeled_files


def build_split(
    files: list[LabeledFile],
    config: Config,
) -> PreparedSplit:
    features: list[np.ndarray] = []
    labels: list[int] = []
    source_paths: list[str] = []
    clip_start_seconds: list[float] = []
    clip_rms: list[float] = []

    for labeled_file in files:
        audio = load_audio(labeled_file.path, config)
        for clip in iter_fixed_clips(
            audio,
            clip_samples=config.clip_samples,
            stride_samples=config.stride_samples,
        ):
            if clip.rms < config.min_clip_rms:
                continue
            features.append(extract_mfcc(clip.audio, config))
            labels.append(labeled_file.label)
            source_paths.append(str(labeled_file.path))
            clip_start_seconds.append(clip.start_sample / config.sample_rate)
            clip_rms.append(clip.rms)

    if not features:
        raise RuntimeError("No usable clips were created. Try lowering min_clip_rms.")

    return PreparedSplit(
        features=np.stack(features),
        labels=np.asarray(labels, dtype=np.int64),
        source_paths=np.asarray(source_paths),
        clip_start_seconds=np.asarray(clip_start_seconds, dtype=np.float32),
        clip_rms=np.asarray(clip_rms, dtype=np.float32),
    )


def balance_training_split(
    split: PreparedSplit,
    config: Config,
) -> PreparedSplit:
    if not config.balance_train_classes:
        return split

    rng = np.random.default_rng(config.random_seed)
    class_indices = [
        np.flatnonzero(split.labels == label)
        for label in range(len(config.classes))
    ]
    target_count = min(len(indices) for indices in class_indices)
    if target_count == 0:
        raise RuntimeError("Cannot balance training split because a class is empty.")

    selected = np.concatenate(
        [
            rng.choice(indices, size=target_count, replace=False)
            for indices in class_indices
        ]
    )
    rng.shuffle(selected)

    return PreparedSplit(
        features=split.features[selected],
        labels=split.labels[selected],
        source_paths=split.source_paths[selected],
        clip_start_seconds=split.clip_start_seconds[selected],
        clip_rms=split.clip_rms[selected],
    )


def print_summary(name: str, split: PreparedSplit, config: Config) -> None:
    counts = {
        class_name: int(np.sum(split.labels == label))
        for label, class_name in enumerate(config.classes)
    }
    print(f"{name}: {len(split.labels)} clips {counts}")


def main() -> None:
    config = load_config()
    labeled_files = collect_labeled_files(config)

    train_files: list[LabeledFile] = []
    val_files: list[LabeledFile] = []
    test_files: list[LabeledFile] = []
    for label, class_name in enumerate(config.classes):
        class_files = [file for file in labeled_files if file.label == label]
        class_train, class_val, class_test = split_class_files(class_files, config)
        train_files.extend(class_train)
        val_files.extend(class_val)
        test_files.extend(class_test)
        print(
            f"{class_name}: "
            f"{len(class_train)} train files, "
            f"{len(class_val)} val files, "
            f"{len(class_test)} test files"
        )

    train = balance_training_split(build_split(train_files, config), config)
    val = build_split(val_files, config)
    test = build_split(test_files, config)

    mean = train.features.mean(axis=(0, 1), keepdims=True)
    std = train.features.std(axis=(0, 1), keepdims=True)

    config.processed_dir.mkdir(parents=True, exist_ok=True)
    np.savez_compressed(
        config.processed_dir / "dataset.npz",
        x_train=train.features,
        y_train=train.labels,
        train_source_paths=train.source_paths,
        train_clip_start_seconds=train.clip_start_seconds,
        train_clip_rms=train.clip_rms,
        x_val=val.features,
        y_val=val.labels,
        val_source_paths=val.source_paths,
        val_clip_start_seconds=val.clip_start_seconds,
        val_clip_rms=val.clip_rms,
        x_test=test.features,
        y_test=test.labels,
        test_source_paths=test.source_paths,
        test_clip_start_seconds=test.clip_start_seconds,
        test_clip_rms=test.clip_rms,
        mean=mean,
        std=std,
        classes=np.asarray(config.classes),
    )

    print_summary("train", train, config)
    print_summary("val", val, config)
    print_summary("test", test, config)
    print(f"Saved dataset to {config.processed_dir / 'dataset.npz'}")


if __name__ == "__main__":
    main()

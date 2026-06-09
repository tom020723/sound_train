from __future__ import annotations

import numpy as np
from sklearn.model_selection import train_test_split

from src.common.config import load_config
from src.data.audio_files import iter_audio_files
from src.features.librosa_mfcc import load_audio, split_fixed_clips
from src.features.cmsis_mfcc import build_mfcc_params, load_cmsis_mfcc_config, mfcc as cmsis_mfcc


def main() -> None:
    config = load_config()
    cmsis_params = build_mfcc_params(load_cmsis_mfcc_config("configs/cmsis_mfcc.json"))

    features: list[np.ndarray] = []
    labels: list[int] = []

    # 파일당 최대 30클립(30초) 추출 — 총 1030클립 기준
    MAX_CLIPS_PER_FILE = 30

    for label, class_name in enumerate(config.classes):
        for audio_path in iter_audio_files(config.data_dir / class_name):
            audio = load_audio(audio_path, config)  # float32 [-1, 1]

            for i, clip in enumerate(split_fixed_clips(audio, config.clip_samples)):
                if i >= MAX_CLIPS_PER_FILE:
                    break

                # CMSIS-DSP와 동일한 알고리즘으로 MFCC 추출 → shape (98, 13)
                features.append(cmsis_mfcc(clip, cmsis_params))
                labels.append(label)

    if not features:
        raise RuntimeError("No audio files found under data/raw.")

    x = np.stack(features)
    y = np.asarray(labels, dtype=np.int64)

    print(f"Dataset: {len(x)} clips, MFCC shape per clip: {x.shape[1:]}")
    print(f"  rain: {(y == 1).sum()}, not_rain: {(y == 0).sum()}")

    x_train, x_test, y_train, y_test = train_test_split(
        x,
        y,
        test_size=config.test_size,
        random_state=config.random_seed,
        stratify=y,
    )
    x_train, x_val, y_train, y_val = train_test_split(
        x_train,
        y_train,
        test_size=config.validation_size,
        random_state=config.random_seed,
        stratify=y_train,
    )

    config.processed_dir.mkdir(parents=True, exist_ok=True)
    np.savez_compressed(
        config.processed_dir / "dataset.npz",
        x_train=x_train,
        y_train=y_train,
        x_val=x_val,
        y_val=y_val,
        x_test=x_test,
        y_test=y_test,
        classes=np.asarray(config.classes),
    )
    print(f"Saved dataset to {config.processed_dir / 'dataset.npz'}")


if __name__ == "__main__":
    main()
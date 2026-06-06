from __future__ import annotations

import numpy as np
from sklearn.model_selection import train_test_split

from src.common.config import load_config
from src.data.audio_files import iter_audio_files
from src.features.librosa_mfcc import extract_mfcc, load_audio, split_fixed_clips


def main() -> None:
    config = load_config()
    features: list[np.ndarray] = []
    labels: list[int] = []
    
    # 변경된 부분: 한 파일당 최대 추출 개수 제한 설정
    MAX_CLIPS_PER_FILE = 30

    for label, class_name in enumerate(config.classes):
        for audio_path in iter_audio_files(config.data_dir / class_name):
            audio = load_audio(audio_path, config)
            
            # 변경된 부분: enumerate를 사용해 클립 개수를 세면서 추출
            for i, clip in enumerate(split_fixed_clips(audio, config.clip_samples)):
                if i >= MAX_CLIPS_PER_FILE:
                    break  # 30개를 채우면 더 이상 추출하지 않고 다음 파일로 넘어감
                
                features.append(extract_mfcc(clip, config))
                labels.append(label)

    if not features:
        raise RuntimeError("No audio files found under data/raw.")

    x = np.stack(features)
    y = np.asarray(labels, dtype=np.int64)

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

    mean = x_train.mean(axis=(0, 1), keepdims=True)
    std = x_train.std(axis=(0, 1), keepdims=True)

    config.processed_dir.mkdir(parents=True, exist_ok=True)
    np.savez_compressed(
        config.processed_dir / "dataset.npz",
        x_train=x_train,
        y_train=y_train,
        x_val=x_val,
        y_val=y_val,
        x_test=x_test,
        y_test=y_test,
        mean=mean,
        std=std,
        classes=np.asarray(config.classes),
    )
    print(f"Saved dataset to {config.processed_dir / 'dataset.npz'}")


if __name__ == "__main__":
    main()
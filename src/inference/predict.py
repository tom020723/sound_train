from __future__ import annotations

import argparse

import numpy as np
import tensorflow as tf

from src.common.config import load_config
from src.features.librosa_mfcc import (
    extract_mfcc,
    load_audio,
    normalize_features,
    split_fixed_clips,
)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("audio_path")
    args = parser.parse_args()

    config = load_config()
    data = np.load(config.processed_dir / "dataset.npz", allow_pickle=True)
    model = tf.keras.models.load_model(config.model_dir / "rain_detector.keras")

    audio = load_audio(args.audio_path, config)
    clips = split_fixed_clips(audio, config.clip_samples)
    features = np.stack([extract_mfcc(clip, config) for clip in clips])
    x = normalize_features(features, data["mean"], data["std"])[..., None]

    probabilities = model.predict(x).mean(axis=0)
    label = config.classes[int(probabilities.argmax())]
    score = float(probabilities.max())
    print(f"{label}: {score:.3f}")


if __name__ == "__main__":
    main()

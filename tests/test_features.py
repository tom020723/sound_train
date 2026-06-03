from __future__ import annotations

import numpy as np

from src.config import Config
from src.features import extract_mfcc, split_fixed_clips


def test_split_fixed_clips_pads_short_audio() -> None:
    clips = split_fixed_clips(np.ones(4, dtype=np.float32), clip_samples=8)
    assert len(clips) == 1
    assert clips[0].shape == (8,)


def test_extract_mfcc_shape() -> None:
    config = Config(
        sample_rate=16000,
        clip_seconds=1.0,
        n_mfcc=13,
        n_fft=512,
        hop_length=160,
        win_length=480,
        classes=["not_rain", "rain"],
        data_dir="data/raw",
        processed_dir="data/processed",
        model_dir="models",
        test_size=0.2,
        validation_size=0.2,
        random_seed=42,
        epochs=1,
        batch_size=4,
    )
    audio = np.zeros(config.clip_samples, dtype=np.float32)
    mfcc = extract_mfcc(audio, config)
    assert mfcc.shape[1] == config.n_mfcc

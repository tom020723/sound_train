from __future__ import annotations

from pathlib import Path

import librosa
import numpy as np

from src.common.config import Config


def load_audio(path: str | Path, config: Config) -> np.ndarray:
    audio, _ = librosa.load(path, sr=config.sample_rate, mono=True)
    return audio.astype(np.float32)


def split_fixed_clips(audio: np.ndarray, clip_samples: int) -> list[np.ndarray]:
    if len(audio) == 0:
        return []
    if len(audio) < clip_samples:
        return [np.pad(audio, (0, clip_samples - len(audio))).astype(np.float32)]

    clips = []
    for start in range(0, len(audio) - clip_samples + 1, clip_samples):
        clips.append(audio[start : start + clip_samples].astype(np.float32))
    return clips


def extract_mfcc(audio: np.ndarray, config: Config) -> np.ndarray:
    mfcc = librosa.feature.mfcc(
        y=audio,
        sr=config.sample_rate,
        n_mfcc=config.n_mfcc,
        n_fft=config.n_fft,
        hop_length=config.hop_length,
        win_length=config.win_length,
    )
    return mfcc.T.astype(np.float32)


def normalize_features(
    features: np.ndarray,
    mean: np.ndarray,
    std: np.ndarray,
) -> np.ndarray:
    return ((features - mean) / np.maximum(std, 1e-6)).astype(np.float32)

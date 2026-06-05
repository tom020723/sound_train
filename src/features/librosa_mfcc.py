from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path

import librosa
import numpy as np

from src.common.config import Config


@dataclass(frozen=True)
class AudioClip:
    audio: np.ndarray
    start_sample: int

    @property
    def rms(self) -> float:
        return float(np.sqrt(np.mean(np.square(self.audio), dtype=np.float64)))


def load_audio(path: str | Path, config: Config) -> np.ndarray:
    audio, _ = librosa.load(path, sr=config.sample_rate, mono=True)
    return audio.astype(np.float32)


def iter_fixed_clips(
    audio: np.ndarray,
    clip_samples: int,
    stride_samples: int | None = None,
) -> list[AudioClip]:
    if len(audio) == 0:
        return []
    if len(audio) < clip_samples:
        padded = np.pad(audio, (0, clip_samples - len(audio))).astype(np.float32)
        return [AudioClip(audio=padded, start_sample=0)]

    step = stride_samples or clip_samples
    clips = []
    for start in range(0, len(audio) - clip_samples + 1, step):
        clips.append(
            AudioClip(
                audio=audio[start : start + clip_samples].astype(np.float32),
                start_sample=start,
            )
        )
    return clips


def split_fixed_clips(
    audio: np.ndarray,
    clip_samples: int,
    stride_samples: int | None = None,
) -> list[np.ndarray]:
    return [
        clip.audio
        for clip in iter_fixed_clips(
            audio=audio,
            clip_samples=clip_samples,
            stride_samples=stride_samples,
        )
    ]


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

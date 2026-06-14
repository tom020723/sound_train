from __future__ import annotations

import json
from dataclasses import dataclass
from pathlib import Path

import numpy as np


@dataclass(frozen=True)
class CmsisMfccConfig:
    sample_rate: int
    frame_length: int
    frame_step: int
    fft_length: int
    num_mel_filters: int
    num_mfcc: int
    lower_frequency_hz: float
    upper_frequency_hz: float
    window_type: str
    log_floor: float
    dct_orthonormal: bool


@dataclass(frozen=True)
class CmsisMfccParams:
    config: CmsisMfccConfig
    dct_coefs: np.ndarray
    filter_pos: np.ndarray
    filter_lengths: np.ndarray
    filter_coefs: np.ndarray
    window_coefs: np.ndarray


def load_cmsis_mfcc_config(
    path: str | Path = "configs/cmsis_mfcc.json",
) -> CmsisMfccConfig:
    raw = json.loads(Path(path).read_text(encoding="utf-8"))
    return CmsisMfccConfig(
        sample_rate=raw["sample_rate"],
        frame_length=raw["frame_length"],
        frame_step=raw["frame_step"],
        fft_length=raw["fft_length"],
        num_mel_filters=raw["num_mel_filters"],
        num_mfcc=raw["num_mfcc"],
        lower_frequency_hz=raw["lower_frequency_hz"],
        upper_frequency_hz=raw["upper_frequency_hz"],
        window_type=raw["window_type"],
        log_floor=raw["log_floor"],
        dct_orthonormal=raw["dct_orthonormal"],
    )


def hz_to_mel(frequency_hz: np.ndarray | float) -> np.ndarray | float:
    return 2595.0 * np.log10(1.0 + np.asarray(frequency_hz) / 700.0)


def mel_to_hz(mel: np.ndarray | float) -> np.ndarray | float:
    return 700.0 * (np.power(10.0, np.asarray(mel) / 2595.0) - 1.0)


def build_window(config: CmsisMfccConfig) -> np.ndarray:
    if config.frame_length > config.fft_length:
        raise ValueError("frame_length must be <= fft_length")

    if config.window_type == "hamming":
        frame_window = np.hamming(config.frame_length)
    elif config.window_type == "hann":
        frame_window = np.hanning(config.frame_length)
    elif config.window_type == "rectangular":
        frame_window = np.ones(config.frame_length)
    else:
        raise ValueError(f"Unsupported window_type: {config.window_type}")

    window = np.zeros(config.fft_length, dtype=np.float32)
    window[: config.frame_length] = frame_window.astype(np.float32)
    return window


def build_dense_mel_filters(config: CmsisMfccConfig) -> np.ndarray:
    num_fft_bins = config.fft_length // 2 + 1
    mel_min = hz_to_mel(config.lower_frequency_hz)
    mel_max = hz_to_mel(config.upper_frequency_hz)
    mel_points = np.linspace(mel_min, mel_max, config.num_mel_filters + 2)
    hz_points = mel_to_hz(mel_points)
    bin_points = np.floor((config.fft_length + 1) * hz_points / config.sample_rate)
    bin_points = np.clip(bin_points.astype(int), 0, num_fft_bins - 1)

    filters = np.zeros((config.num_mel_filters, num_fft_bins), dtype=np.float32)
    for index in range(config.num_mel_filters):
        left = bin_points[index]
        center = bin_points[index + 1]
        right = bin_points[index + 2]

        if center > left:
            filters[index, left:center] = (
                np.arange(left, center, dtype=np.float32) - left
            ) / (center - left)
        if right > center:
            filters[index, center:right] = (
                right - np.arange(center, right, dtype=np.float32)
            ) / (right - center)

    return filters


def dense_to_sparse_filters(
    filters: np.ndarray,
) -> tuple[np.ndarray, np.ndarray, np.ndarray]:
    positions: list[int] = []
    lengths: list[int] = []
    coefficients: list[np.float32] = []

    for row in filters:
        nonzero = np.flatnonzero(row > 0.0)
        if len(nonzero) == 0:
            positions.append(0)
            lengths.append(0)
            continue

        start = int(nonzero[0])
        stop = int(nonzero[-1]) + 1
        positions.append(start)
        lengths.append(stop - start)
        coefficients.extend(row[start:stop].astype(np.float32))

    return (
        np.asarray(positions, dtype=np.int32),
        np.asarray(lengths, dtype=np.int32),
        np.asarray(coefficients, dtype=np.float32),
    )


def build_dct_coefs(config: CmsisMfccConfig) -> np.ndarray:
    mel_index = np.arange(config.num_mel_filters, dtype=np.float32)
    dct = np.zeros((config.num_mfcc, config.num_mel_filters), dtype=np.float32)
    for mfcc_index in range(config.num_mfcc):
        dct[mfcc_index] = np.cos(
            np.pi
            * mfcc_index
            * (mel_index + 0.5)
            / config.num_mel_filters
        )

    if config.dct_orthonormal:
        dct[0] *= np.sqrt(1.0 / config.num_mel_filters)
        dct[1:] *= np.sqrt(2.0 / config.num_mel_filters)

    return dct.astype(np.float32)


def build_mfcc_params(config: CmsisMfccConfig) -> CmsisMfccParams:
    filters = build_dense_mel_filters(config)
    filter_pos, filter_lengths, filter_coefs = dense_to_sparse_filters(filters)
    return CmsisMfccParams(
        config=config,
        dct_coefs=build_dct_coefs(config),
        filter_pos=filter_pos,
        filter_lengths=filter_lengths,
        filter_coefs=filter_coefs,
        window_coefs=build_window(config),
    )


def frame_audio(audio: np.ndarray, config: CmsisMfccConfig) -> list[tuple[int, np.ndarray]]:
    if len(audio) == 0:
        return []

    if len(audio) < config.frame_length:
        padded = np.pad(audio, (0, config.frame_length - len(audio)))
        return [(0, padded.astype(np.float32))]

    frames: list[tuple[int, np.ndarray]] = []
    for start in range(0, len(audio) - config.frame_length + 1, config.frame_step):
        frames.append((start, audio[start : start + config.frame_length].astype(np.float32)))
    return frames


def mfcc_frame(
    frame: np.ndarray,
    params: CmsisMfccParams,
) -> np.ndarray:
    config = params.config
    fft_input = np.zeros(config.fft_length, dtype=np.float32)
    fft_input[: len(frame)] = frame[: config.frame_length]

    max_value = np.max(np.abs(fft_input))
    if max_value != 0.0:
        fft_input *= np.float32(1.0 / max_value)

    fft_input *= params.window_coefs

    magnitude_spectrum = np.abs(np.fft.rfft(fft_input, n=config.fft_length)).astype(
        np.float32
    )
    if max_value != 0.0:
        magnitude_spectrum *= np.float32(max_value)

    mel_energies = np.zeros(config.num_mel_filters, dtype=np.float32)
    offset = 0
    for index, length in enumerate(params.filter_lengths):
        start = params.filter_pos[index]
        stop = offset + length
        coefs = params.filter_coefs[offset:stop]
        bins = magnitude_spectrum[start : start + length]
        mel_energies[index] = float(np.dot(coefs, bins))
        offset = stop

    log_mel = np.log(np.maximum(mel_energies, config.log_floor)).astype(np.float32)
    return np.dot(params.dct_coefs, log_mel).astype(np.float32)


def mfcc(audio: np.ndarray, params: CmsisMfccParams) -> np.ndarray:
    frames = frame_audio(audio, params.config)
    if not frames:
        return np.empty((0, params.config.num_mfcc), dtype=np.float32)
    return np.stack([mfcc_frame(frame, params) for _, frame in frames])

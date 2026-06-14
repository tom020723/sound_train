from __future__ import annotations

import argparse
from pathlib import Path

import numpy as np

from src.features.cmsis_mfcc import build_mfcc_params, load_cmsis_mfcc_config, mfcc
from src.tools.export_mfcc_params import render_header


def generate_test_signal(sample_rate: int, seconds: float = 1.0) -> np.ndarray:
    sample_count = int(sample_rate * seconds)
    time = np.arange(sample_count, dtype=np.float32) / sample_rate
    signal = (
        0.45 * np.sin(2.0 * np.pi * 440.0 * time)
        + 0.20 * np.sin(2.0 * np.pi * 1300.0 * time)
        + 0.05 * np.sin(2.0 * np.pi * 3100.0 * time)
    )
    ramp = np.linspace(0.2, 1.0, sample_count, dtype=np.float32)
    return (signal * ramp).astype(np.float32)


def float_to_int16_pcm(audio: np.ndarray) -> np.ndarray:
    clipped = np.clip(audio, -1.0, 1.0)
    return np.round(clipped * 32767.0).astype(np.int16)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--config", default="configs/cmsis_mfcc.json")
    parser.add_argument("--output-dir", type=Path, default=Path("artifacts/mfcc_golden"))
    args = parser.parse_args()

    config = load_cmsis_mfcc_config(args.config)
    params = build_mfcc_params(config)
    audio = generate_test_signal(config.sample_rate)
    pcm = float_to_int16_pcm(audio)
    mfcc_values = mfcc(pcm.astype(np.float32) / 32768.0, params)

    args.output_dir.mkdir(parents=True, exist_ok=True)
    (args.output_dir / "input_pcm_i16.raw").write_bytes(pcm.tobytes())
    np.savetxt(args.output_dir / "input_pcm_i16.csv", pcm, fmt="%d", delimiter=",")
    np.savetxt(args.output_dir / "python_mfcc.csv", mfcc_values, fmt="%.9e", delimiter=",")
    (args.output_dir / "mfcc_params.h").write_text(render_header(params), encoding="utf-8")

    print(f"Saved golden input/output to {args.output_dir}")
    print(f"PCM samples: {len(pcm)}")
    print(f"MFCC shape: {mfcc_values.shape}")


if __name__ == "__main__":
    main()

from __future__ import annotations

import argparse
from pathlib import Path

import numpy as np


def load_numeric_csv(path: Path) -> np.ndarray:
    rows: list[list[float]] = []
    for raw_line in path.read_text(encoding="utf-8", errors="ignore").splitlines():
        line = raw_line.strip()
        if not line or "," not in line:
            continue
        try:
            rows.append([float(value.strip()) for value in line.split(",")])
        except ValueError:
            continue

    if not rows:
        raise ValueError(f"No numeric CSV rows found in {path}")
    return np.asarray(rows, dtype=np.float32)


def cosine_similarity(a: np.ndarray, b: np.ndarray) -> float:
    a_flat = a.reshape(-1)
    b_flat = b.reshape(-1)
    denominator = np.linalg.norm(a_flat) * np.linalg.norm(b_flat)
    if denominator == 0.0:
        return 0.0
    return float(np.dot(a_flat, b_flat) / denominator)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("python_mfcc", type=Path)
    parser.add_argument("stm_mfcc", type=Path)
    args = parser.parse_args()

    python_mfcc = load_numeric_csv(args.python_mfcc)
    stm_mfcc = load_numeric_csv(args.stm_mfcc)

    if python_mfcc.shape != stm_mfcc.shape:
        raise ValueError(
            f"Shape mismatch: python={python_mfcc.shape}, stm={stm_mfcc.shape}"
        )

    diff = python_mfcc - stm_mfcc
    print(f"shape: {python_mfcc.shape}")
    print(f"max_abs_error: {np.max(np.abs(diff)):.9e}")
    print(f"mean_abs_error: {np.mean(np.abs(diff)):.9e}")
    print(f"rmse: {np.sqrt(np.mean(diff ** 2)):.9e}")
    print(f"cosine_similarity: {cosine_similarity(python_mfcc, stm_mfcc):.9f}")


if __name__ == "__main__":
    main()

from __future__ import annotations

import argparse
from pathlib import Path

import numpy as np


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

    python_mfcc = np.loadtxt(args.python_mfcc, delimiter=",", dtype=np.float32)
    stm_mfcc = np.loadtxt(args.stm_mfcc, delimiter=",", dtype=np.float32)

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

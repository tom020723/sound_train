from __future__ import annotations

import argparse
from pathlib import Path

import numpy as np


def format_i16_array(name: str, values: np.ndarray, columns: int = 10) -> str:
    flat = values.reshape(-1)
    lines = [f"static const int16_t {name}[{len(flat)}] = {{"]
    for start in range(0, len(flat), columns):
        chunk = flat[start : start + columns]
        lines.append("    " + ", ".join(str(int(value)) for value in chunk) + ",")
    lines.append("};")
    return "\n".join(lines)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--input",
        type=Path,
        default=Path("artifacts/mfcc_golden/input_pcm_i16.raw"),
    )
    parser.add_argument(
        "--output",
        type=Path,
        default=Path("artifacts/mfcc_golden/input_pcm_i16.h"),
    )
    parser.add_argument("--array-name", default="mfcc_golden_pcm_i16")
    args = parser.parse_args()

    pcm = np.frombuffer(args.input.read_bytes(), dtype=np.int16)
    header = "\n\n".join(
        [
            "#pragma once",
            "#include <stdint.h>",
            f"#define MFCC_GOLDEN_PCM_LENGTH {len(pcm)}",
            format_i16_array(args.array_name, pcm),
            "",
        ]
    )
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(header, encoding="utf-8")
    print(f"Saved PCM header to {args.output}")


if __name__ == "__main__":
    main()

from __future__ import annotations

import argparse
from pathlib import Path

import numpy as np

from src.features.cmsis_mfcc import (
    CmsisMfccParams,
    build_mfcc_params,
    load_cmsis_mfcc_config,
)


def format_float_array(name: str, values: np.ndarray, columns: int = 6) -> str:
    flat = values.reshape(-1)
    lines = [f"static const float32_t {name}[{len(flat)}] = {{"]
    for start in range(0, len(flat), columns):
        chunk = flat[start : start + columns]
        lines.append("    " + ", ".join(f"{value:.9e}f" for value in chunk) + ",")
    lines.append("};")
    return "\n".join(lines)


def format_int_array(name: str, values: np.ndarray, columns: int = 12) -> str:
    flat = values.reshape(-1)
    lines = [f"static const uint32_t {name}[{len(flat)}] = {{"]
    for start in range(0, len(flat), columns):
        chunk = flat[start : start + columns]
        lines.append("    " + ", ".join(str(int(value)) for value in chunk) + ",")
    lines.append("};")
    return "\n".join(lines)


def render_header(params: CmsisMfccParams) -> str:
    config = params.config
    return "\n\n".join(
        [
            "#pragma once",
            "#include <stdint.h>",
            '#include "arm_math.h"',
            f"#define MFCC_SAMPLE_RATE {config.sample_rate}",
            f"#define MFCC_FRAME_LENGTH {config.frame_length}",
            f"#define MFCC_FRAME_STEP {config.frame_step}",
            f"#define MFCC_FFT_LENGTH {config.fft_length}",
            f"#define MFCC_NUM_MEL_FILTERS {config.num_mel_filters}",
            f"#define MFCC_NUM_COEFS {config.num_mfcc}",
            f"#define MFCC_FILTER_COEFS_LENGTH {len(params.filter_coefs)}",
            format_float_array("mfcc_dct_coefs", params.dct_coefs),
            format_int_array("mfcc_filter_pos", params.filter_pos),
            format_int_array("mfcc_filter_lengths", params.filter_lengths),
            format_float_array("mfcc_filter_coefs", params.filter_coefs),
            format_float_array("mfcc_window_coefs", params.window_coefs),
            "",
        ]
    )


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--config", default="configs/cmsis_mfcc.json")
    parser.add_argument("--output", type=Path, default=Path("artifacts/mfcc_params.h"))
    args = parser.parse_args()

    params = build_mfcc_params(load_cmsis_mfcc_config(args.config))
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(render_header(params), encoding="utf-8")
    print(f"Saved MFCC params header to {args.output}")


if __name__ == "__main__":
    main()

from __future__ import annotations

import numpy as np

from src.features.cmsis_mfcc import (
    build_mfcc_params,
    load_cmsis_mfcc_config,
    mfcc,
)


def test_cmsis_mfcc_shape() -> None:
    config = load_cmsis_mfcc_config()
    params = build_mfcc_params(config)
    audio = np.zeros(config.sample_rate, dtype=np.float32)

    features = mfcc(audio, params)

    assert features.shape[1] == config.num_mfcc
    assert features.shape[0] == 98


def test_cmsis_mfcc_is_deterministic() -> None:
    config = load_cmsis_mfcc_config()
    params = build_mfcc_params(config)
    audio = np.linspace(-0.5, 0.5, config.sample_rate, dtype=np.float32)

    first = mfcc(audio, params)
    second = mfcc(audio, params)

    np.testing.assert_allclose(first, second)

from __future__ import annotations

import json
from dataclasses import dataclass
from pathlib import Path


@dataclass(frozen=True)
class Config:
    sample_rate: int
    clip_seconds: float
    n_mfcc: int
    n_fft: int
    hop_length: int
    win_length: int
    classes: list[str]
    data_dir: Path
    processed_dir: Path
    model_dir: Path
    test_size: float
    validation_size: float
    random_seed: int
    epochs: int
    batch_size: int

    @property
    def clip_samples(self) -> int:
        return int(self.sample_rate * self.clip_seconds)


def load_config(path: str | Path = "configs/default.json") -> Config:
    raw = json.loads(Path(path).read_text(encoding="utf-8"))
    return Config(
        sample_rate=raw["sample_rate"],
        clip_seconds=raw["clip_seconds"],
        n_mfcc=raw["n_mfcc"],
        n_fft=raw["n_fft"],
        hop_length=raw["hop_length"],
        win_length=raw["win_length"],
        classes=list(raw["classes"]),
        data_dir=Path(raw["data_dir"]),
        processed_dir=Path(raw["processed_dir"]),
        model_dir=Path(raw["model_dir"]),
        test_size=raw["test_size"],
        validation_size=raw["validation_size"],
        random_seed=raw["random_seed"],
        epochs=raw["epochs"],
        batch_size=raw["batch_size"],
    )

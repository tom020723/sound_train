from __future__ import annotations

import json
import os
import shutil
import tempfile
from pathlib import Path

import numpy as np

from src.common.config import load_config
from src.features.librosa_mfcc import normalize_features
from src.models.cnn import build_model


def use_model_local_temp(model_dir: Path) -> Path:
    temp_dir = tempfile.mkdtemp(prefix="keras-save-", dir=model_dir)
    os.environ["TEMP"] = temp_dir
    os.environ["TMP"] = temp_dir
    tempfile.tempdir = temp_dir
    return Path(temp_dir)


def main() -> None:
    config = load_config()
    data = np.load(config.processed_dir / "dataset.npz", allow_pickle=True)

    x_train = normalize_features(data["x_train"], data["mean"], data["std"])[..., None]
    x_val = normalize_features(data["x_val"], data["mean"], data["std"])[..., None]
    y_train = data["y_train"]
    y_val = data["y_val"]

    config.model_dir.mkdir(parents=True, exist_ok=True)
    temp_dir = use_model_local_temp(config.model_dir)

    try:
        model = build_model(x_train.shape[1:], len(config.classes))
        history = model.fit(
            x_train,
            y_train,
            validation_data=(x_val, y_val),
            epochs=config.epochs,
            batch_size=config.batch_size,
            verbose=2,
        )

        model.save(config.model_dir / "rain_detector.keras")
        (config.model_dir / "training_history.json").write_text(
            json.dumps(history.history, indent=2),
            encoding="utf-8",
        )
    finally:
        shutil.rmtree(temp_dir, ignore_errors=True)

    print(f"Saved model to {config.model_dir / 'rain_detector.keras'}")
    print(f"Saved history to {config.model_dir / 'training_history.json'}")


if __name__ == "__main__":
    main()

from __future__ import annotations

import numpy as np

from src.config import load_config
from src.features import normalize_features
from src.model import build_model


def main() -> None:
    config = load_config()
    data = np.load(config.processed_dir / "dataset.npz", allow_pickle=True)

    x_train = normalize_features(data["x_train"], data["mean"], data["std"])[..., None]
    x_val = normalize_features(data["x_val"], data["mean"], data["std"])[..., None]
    y_train = data["y_train"]
    y_val = data["y_val"]

    model = build_model(x_train.shape[1:], len(config.classes))
    model.fit(
        x_train,
        y_train,
        validation_data=(x_val, y_val),
        epochs=config.epochs,
        batch_size=config.batch_size,
    )

    config.model_dir.mkdir(parents=True, exist_ok=True)
    model.save(config.model_dir / "rain_detector.keras")
    print(f"Saved model to {config.model_dir / 'rain_detector.keras'}")


if __name__ == "__main__":
    main()

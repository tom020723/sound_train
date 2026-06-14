from __future__ import annotations

import numpy as np

from src.common.config import load_config
from src.models.cnn import build_model


def main() -> None:
    config = load_config()
    data = np.load(config.processed_dir / "dataset.npz", allow_pickle=True)

    # Raw CMSIS-style MFCC values are used so training/export/inference share the same input scale.
    x_train = data["x_train"][..., None]
    x_val   = data["x_val"][..., None]
    y_train = data["y_train"]
    y_val   = data["y_val"]

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

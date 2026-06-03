from __future__ import annotations

import numpy as np
import tensorflow as tf
from sklearn.metrics import classification_report, confusion_matrix

from src.config import load_config
from src.features import normalize_features


def main() -> None:
    config = load_config()
    data = np.load(config.processed_dir / "dataset.npz", allow_pickle=True)
    model = tf.keras.models.load_model(config.model_dir / "rain_detector.keras")

    x_test = normalize_features(data["x_test"], data["mean"], data["std"])[..., None]
    y_test = data["y_test"]
    y_pred = model.predict(x_test).argmax(axis=1)

    print(confusion_matrix(y_test, y_pred))
    print(classification_report(y_test, y_pred, target_names=config.classes))


if __name__ == "__main__":
    main()

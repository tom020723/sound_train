from __future__ import annotations

import numpy as np
import tensorflow as tf
import matplotlib.pyplot as plt
from sklearn.metrics import classification_report, confusion_matrix

from src.common.config import load_config


def plot_confusion_matrix(cm: np.ndarray, classes: list[str], output_path: str) -> None:
    fig, ax = plt.subplots(figsize=(6, 5))
    im = ax.imshow(cm, interpolation="nearest", cmap="Blues")
    fig.colorbar(im, ax=ax)

    ax.set(
        xticks=range(len(classes)),
        yticks=range(len(classes)),
        xticklabels=classes,
        yticklabels=classes,
        xlabel="Predicted label",
        ylabel="True label",
        title="Confusion Matrix",
    )

    thresh = cm.max() / 2.0
    for i in range(cm.shape[0]):
        for j in range(cm.shape[1]):
            ax.text(j, i, cm[i, j], ha="center", va="center",
                    color="white" if cm[i, j] > thresh else "black", fontsize=14)

    fig.tight_layout()
    fig.savefig(output_path, dpi=150)
    print(f"Saved confusion matrix → {output_path}")
    plt.close(fig)


def main() -> None:
    config = load_config()
    data = np.load(config.processed_dir / "dataset.npz", allow_pickle=True)
    model = tf.keras.models.load_model(config.model_dir / "rain_detector.keras")

    x_test = data["x_test"][..., None]
    y_test = data["y_test"]
    y_pred = model.predict(x_test).argmax(axis=1)

    cm = confusion_matrix(y_test, y_pred)
    print(cm)
    print(classification_report(y_test, y_pred, target_names=list(config.classes)))

    out = str(config.model_dir / "confusion_matrix.png")
    plot_confusion_matrix(cm, list(config.classes), out)


if __name__ == "__main__":
    main()

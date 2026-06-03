from __future__ import annotations

import numpy as np
import tensorflow as tf

from src.config import load_config
from src.features import normalize_features


def main() -> None:
    config = load_config()
    data = np.load(config.processed_dir / "dataset.npz", allow_pickle=True)
    model_path = config.model_dir / "rain_detector.keras"
    output_path = config.model_dir / "rain_detector_int8.tflite"

    x_train = normalize_features(data["x_train"], data["mean"], data["std"])[..., None]

    def representative_dataset():
        for sample in x_train[: min(len(x_train), 100)]:
            yield [sample[None, ...].astype(np.float32)]

    converter = tf.lite.TFLiteConverter.from_keras_model(
        tf.keras.models.load_model(model_path)
    )
    converter.optimizations = [tf.lite.Optimize.DEFAULT]
    converter.representative_dataset = representative_dataset
    converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS_INT8]
    converter.inference_input_type = tf.int8
    converter.inference_output_type = tf.int8

    output_path.write_bytes(converter.convert())
    print(f"Saved int8 TFLite model to {output_path}")


if __name__ == "__main__":
    main()

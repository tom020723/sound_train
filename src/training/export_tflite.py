from __future__ import annotations

import numpy as np
import tensorflow as tf
from pathlib import Path

from src.common.config import load_config


def convert_to_c_array(tflite_model: bytes, output_path: Path, array_name: str = "g_model") -> None:
    hex_array = [f"0x{b:02x}" for b in tflite_model]
    lines = []
    for i in range(0, len(hex_array), 12):
        lines.append("    " + ", ".join(hex_array[i:i+12]))
    c_code  = f"// Automatically generated TFLite INT8 model array\n"
    c_code += f"// Model size: {len(tflite_model)} bytes\n\n"
    c_code += f"const unsigned char {array_name}[] = {{\n"
    c_code += ",\n".join(lines) + "\n"
    c_code += f"}};\n\n"
    c_code += f"const unsigned int {array_name}_len = {len(tflite_model)};\n"
    output_path.write_text(c_code, encoding="utf-8")


def main() -> None:
    config = load_config()
    data = np.load(config.processed_dir / "dataset.npz", allow_pickle=True)
    model_path        = config.model_dir / "rain_detector.keras"
    tflite_output_path = config.model_dir / "rain_detector_int8.tflite"
    header_output_path = config.model_dir / "rain_detector_int8.h"

    # 정규화 없이 raw CMSIS MFCC — train.py와 동일하게 맞춤
    # train + val 전체 사용: BN 레이어가 있는 모델은 충분한 캘리브레이션 데이터 필요
    x_all = np.concatenate([data["x_train"][..., None], data["x_val"][..., None]], axis=0)

    np.random.seed(42)
    indices = np.random.permutation(len(x_all))
    subset_x = x_all[indices]

    def representative_dataset():
        for sample in subset_x:
            yield [sample[None, ...].astype(np.float32)]

    model = tf.keras.models.load_model(str(model_path))
    converter = tf.lite.TFLiteConverter.from_keras_model(model)
    converter.optimizations = [tf.lite.Optimize.DEFAULT]
    converter.representative_dataset = representative_dataset
    converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS_INT8]
    converter.inference_input_type  = tf.int8
    converter.inference_output_type = tf.int8

    tflite_model = converter.convert()
    tflite_output_path.write_bytes(tflite_model)
    print(f"TFLite model: {len(tflite_model)/1024:.1f} KB → {tflite_output_path}")

    convert_to_c_array(tflite_model, header_output_path, "rain_detector_tflite")
    print(f"C header: {header_output_path}")
    

if __name__ == "__main__":
    main()
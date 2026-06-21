from __future__ import annotations

from pathlib import Path
import numpy as np
import tensorflow as tf

from src.common.config import load_config
from src.features.cmsis_mfcc import (
    build_mfcc_params,
    load_cmsis_mfcc_config,
    mfcc as cmsis_mfcc,
)
from src.features.librosa_mfcc import (
    load_audio,
    split_fixed_clips,
)

def main() -> None:
    config = load_config()
    cmsis_params = build_mfcc_params(load_cmsis_mfcc_config("configs/cmsis_mfcc.json"))

    keras_model_path = config.model_dir / "rain_detector.keras"
    tflite_model_path = config.model_dir / "rain_detector_int8.tflite"

    if not keras_model_path.exists() or not tflite_model_path.exists():
        print("Model files not found.")
        return

    model_keras = tf.keras.models.load_model(keras_model_path)

    interpreter = tf.lite.Interpreter(model_path=str(tflite_model_path))
    interpreter.allocate_tensors()

    input_details = interpreter.get_input_details()[0]
    output_details = interpreter.get_output_details()[0]

    input_scale, input_zp = input_details['quantization']
    output_scale, output_zp = output_details['quantization']

    test_dir = Path("drive/test_data")
    valid_extensions = {".wav", ".mp3", ".aiff", ".aif", ".flac"}

    test_files = [
        f for f in test_dir.rglob("*")
        if f.is_file() and f.suffix.lower() in valid_extensions
    ]

    if not test_files:
        print(f"No audio files found in '{test_dir}'.")
        return

    print("=======================================================================")
    print(" [Model Comparison] Float32 (Keras) vs INT8 (TFLite)")
    print("=======================================================================")
    print(f" TFLite Input  -> Scale: {input_scale:.8f}, ZeroPoint: {input_zp}")
    print(f" TFLite Output -> Scale: {output_scale:.8f}, ZeroPoint: {output_zp}")
    print("=======================================================================\n")

    for audio_path in test_files:
        print(f"File: {audio_path.name}")

        try:
            audio = load_audio(audio_path, config)

            max_val = np.max(audio)
            min_val = np.min(audio)
            print(f"   max_amp: {max_val:.4f}, min_amp: {min_val:.4f}")

            if max_val == 0.0 and min_val == 0.0:
                print("   Warning: audio is silent.")

        except Exception as e:
            print(f"   Failed to load: {e}\n")
            continue

        clips = split_fixed_clips(audio, config.clip_samples)

        if len(clips) == 0:
            print("   File too short, skipping.\n")
            continue

        features = np.stack([cmsis_mfcc(clip.astype(np.float32), cmsis_params) for clip in clips])
        x_float = features[..., None]

        print(f"   x max: {np.max(x_float):.4f}, min: {np.min(x_float):.4f}")

        probabilities_keras = model_keras.predict(x_float, verbose=0)

        print(f"   {len(clips)} clips")
        print("-" * 85)
        print(f" {'Time':^13} | {'[Keras Float32]':^26} | {'[TFLite INT8]':^36}")
        print(f" {'(sec)':^13} | {'Not Rain':<10} {'Rain':<12} | {'Not Rain':<10} {'Rain':<12} {'(Raw INT8)'}")
        print("-" * 85)

        for i, sample in enumerate(x_float):
            k_not_rain = probabilities_keras[i][0] * 100
            k_rain = probabilities_keras[i][1] * 100

            # quantize to INT8 matching STM32 inference input
            q_sample = np.clip(np.round(sample / input_scale + input_zp), -128, 127).astype(np.int8)

            interpreter.set_tensor(input_details['index'], np.array([q_sample]))
            interpreter.invoke()
            tflite_output = interpreter.get_tensor(output_details['index'])[0]

            print(
                f"DEBUG [{i:02d}s] "
                f"Raw INT8 = [{int(tflite_output[0])}, {int(tflite_output[1])}]"
            )

            # STM32 dashboard formula: (out_int8 + 128) * 100 / 255
            t_not_rain = int((int(tflite_output[0]) + 128) * 100 / 255)
            t_rain = int((int(tflite_output[1]) + 128) * 100 / 255)

            # normalize to sum to 100 to correct integer rounding error
            total_prob = t_not_rain + t_rain
            if total_prob > 0:
                t_not_rain = int((t_not_rain / total_prob) * 100)
                t_rain = 100 - t_not_rain

            time_str = f"{i:02d}s ~ {i+1:02d}s"
            print(f" {time_str:<13} | {k_not_rain:>8.1f}% {k_rain:>10.1f}% | {t_not_rain:>8}% {t_rain:>10}%  (Raw: {list(tflite_output)})")

        overall_prob = probabilities_keras.mean(axis=0)
        overall_label = config.classes[int(overall_prob.argmax())]
        print("-" * 85)
        print(f" Keras prediction: {overall_label}")
        print("=======================================================================\n")


if __name__ == "__main__":
    main()

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
    
    # 1. 🚀 원본 Keras 모델과 양자화 TFLite 모델 동시 로드
    keras_model_path = config.model_dir / "rain_detector.keras"
    tflite_model_path = config.model_dir / "rain_detector_int8.tflite"

    if not keras_model_path.exists() or not tflite_model_path.exists():
        print("❌ 모델 파일을 찾을 수 없습니다. 경로를 확인해주세요.")
        return

    model_keras = tf.keras.models.load_model(keras_model_path)
    
    interpreter = tf.lite.Interpreter(model_path=str(tflite_model_path))
    interpreter.allocate_tensors()
    
    input_details = interpreter.get_input_details()[0]
    output_details = interpreter.get_output_details()[0]
    
    # TFLite 모델 고유의 입력 Scale과 ZeroPoint 추출 (양자화 역산용)
    input_scale, input_zp = input_details['quantization']
    output_scale, output_zp = output_details['quantization']

    test_dir = Path("drive/test_data")
    valid_extensions = {".wav", ".mp3", ".aiff", ".aif", ".flac"}
    
    test_files = [
        f for f in test_dir.rglob("*") 
        if f.is_file() and f.suffix.lower() in valid_extensions
    ]

    if not test_files:
        print(f"❌ '{test_dir}' 폴더 안에 분석할 오디오 파일(.wav, .mp3, .aiff 등)이 없습니다!")
        return

    print("=======================================================================")
    print(" 🎯 [모델 비교 분석 시스템] 원본(Float32) vs 양자화(Int8) 정확도 검증")
    print("=======================================================================")
    print(f" 📊 TFLite Input  -> Scale: {input_scale:.8f}, ZeroPoint: {input_zp}")
    print(f" 📊 TFLite Output -> Scale: {output_scale:.8f}, ZeroPoint: {output_zp}")
    print("=======================================================================\n")

    for audio_path in test_files:
        print(f"▶ 분석 중인 파일: {audio_path.name}")
        
        try:
            audio = load_audio(audio_path, config)
            
            max_val = np.max(audio)
            min_val = np.min(audio)
            print(f"   🔎 [데이터 검사] 최대 진폭: {max_val:.4f}, 최소 진폭: {min_val:.4f}")
            
            if max_val == 0.0 and min_val == 0.0:
                print("   🚨 [경고] 오디오 데이터가 텅 비어있습니다(무음)! 파이썬이 이 파일을 제대로 읽지 못했습니다.")
                
        except Exception as e:
            print(f"   ❌ 파일을 불러오지 못했습니다. (에러: {e})\n")
            continue
        
        clips = split_fixed_clips(audio, config.clip_samples)
        
        if len(clips) == 0:
            print("   ⚠️ 파일 길이가 너무 짧아 분석을 건너뜁니다.\n")
            continue

        # MFCC 및 정규화 진행 (Float32 입력 데이터 생성)
        features = np.stack([cmsis_mfcc(clip.astype(np.float32), cmsis_params) for clip in clips])
        x_float = features[..., None]

        print(f"   🔎 [정규화 검사] 모델 입력값 x의 최댓값: {np.max(x_float):.4f}, 최솟값: {np.min(x_float):.4f}")

        # 2. 🧠 [Keras] 원본 모델 예측 진행
        probabilities_keras = model_keras.predict(x_float, verbose=0)
        
        print(f"   총 {len(clips)}개의 구간으로 분리되었습니다.")
        print("-" * 85)
        print(f" {'시간 구간':^13} | {'[Raw Keras Model (Float32)]':^26} | {'[Quantized TFLite (Int8)]':^36}")
        print(f" {'(초)':^13} | {'Not Rain':<10} {'Rain':<12} | {'Not Rain':<10} {'Rain':<12} {'(Raw INT8)'}")
        print("-" * 85)
        
        # 3. 🧠 [TFLite] 초 단위 루프를 돌며 보드와 똑같이 Int8 연산 수행 및 나란히 출력
        for i, sample in enumerate(x_float):
            # [Keras 결과 확률 계산]
            k_not_rain = probabilities_keras[i][0] * 100
            k_rain = probabilities_keras[i][1] * 100

            # [TFLite 변환] 입력 feature 데이터를 보드 환경과 동일하게 정수형(-128~127)으로 양자화
            q_sample = np.clip(np.round(sample / input_scale + input_zp), -128, 127).astype(np.int8)
            
            # TFLite 인터프리터 실행
            interpreter.set_tensor(input_details['index'], np.array([q_sample]))
            interpreter.invoke()
            tflite_output = interpreter.get_tensor(output_details['index'])[0]

            print(
                f"DEBUG [{i:02d}s] "
                f"Raw INT8 = [{int(tflite_output[0])}, {int(tflite_output[1])}]"
            )

            # STM32 보드 대시보드 화면 연산 수식 그대로 대입: (out_int8 + 128) * 100 / 255
            t_not_rain = int((int(tflite_output[0]) + 128) * 100 / 255)
            t_rain = int((int(tflite_output[1]) + 128) * 100 / 255)

            # 정수 변환 오차로 인해 두 확률의 합이 100%를 미세하게 어긋날 때를 위한 보정
            total_prob = t_not_rain + t_rain
            if total_prob > 0:
                t_not_rain = int((t_not_rain / total_prob) * 100)
                t_rain = 100 - t_not_rain

            # 4. 📊 한 줄에 두 모델의 예측 결과를 매칭하여 출력
            time_str = f"{i:02d}s ~ {i+1:02d}s"
            print(f" {time_str:<13} | {k_not_rain:>8.1f}% {k_rain:>10.1f}% | {t_not_rain:>8}% {t_rain:>10}%  (Raw: {list(tflite_output)})")
            
        # 최종 평균 결론 계산 (Keras 원본 기준)
        overall_prob = probabilities_keras.mean(axis=0)
        overall_label = config.classes[int(overall_prob.argmax())]
        print("-" * 85)
        print(f" ➔ 📌 이 파일의 원본 Keras 최종 평균 결론: {overall_label}")
        print("=======================================================================\n")

    

if __name__ == "__main__":
    main()

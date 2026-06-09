import numpy as np
import tensorflow as tf
import matplotlib.pyplot as plt
import librosa
from pathlib import Path

from src.common.config import load_config
from src.data.audio_files import iter_audio_files
from src.features.librosa_mfcc import load_audio, split_fixed_clips, extract_mfcc, normalize_features

def plot_and_save_mfcc(mfcc_data, title, save_path):
    """MFCC 데이터를 이미지로 그려서 저장하는 함수"""
    plt.figure(figsize=(10, 4))
    # MFCC 배열을 전치(.T)하여 librosa.display에 맞춤
    librosa.display.specshow(mfcc_data.T, x_axis='time')
    plt.colorbar(format='%+2.0f dB')
    plt.title(title)
    plt.tight_layout()
    plt.savefig(save_path)
    plt.close()

def main():
    config = load_config()
    model = tf.keras.models.load_model(config.model_dir / "rain_detector.keras")
    
    # 정규화(Normalize)를 위해 저장해둔 평균과 표준편차 불러오기
    data = np.load(config.processed_dir / "dataset.npz", allow_pickle=True)
    mean, std = data["mean"], data["std"]

    # 틀린 이미지를 저장할 폴더 생성
    error_dir = Path("error_analysis")
    error_dir.mkdir(exist_ok=True)

    print("🔍 틀린 케이스 분석을 시작합니다. 시간이 조금 걸릴 수 있습니다...\n")
    error_count = 0

    for true_label_idx, class_name in enumerate(config.classes):
        # 원본 데이터 폴더에서 오디오 파일을 하나씩 가져옴
        files = iter_audio_files(config.data_dir / class_name)
        
        for file_path in files:
            try:
                audio = load_audio(file_path, config)
            except Exception:
                continue
            
            # 오디오를 1초 단위로 자름
            clips = list(split_fixed_clips(audio, config.clip_samples))
            
            for clip_idx, clip in enumerate(clips):
                # 1. 특징 추출 및 정규화
                mfcc = extract_mfcc(clip, config)
                norm_mfcc = normalize_features(np.array([mfcc]), mean, std)[0]
                
                # 2. 모델 예측 (채널 차원 추가 필요: [..., None])
                pred_probs = model.predict(norm_mfcc[None, ..., None], verbose=0)
                pred_label_idx = pred_probs.argmax(axis=1)[0]
                
                # 3. 예측이 틀렸을 경우 (오답 노트 작성)
                if pred_label_idx != true_label_idx:
                    error_count += 1
                    pred_class = config.classes[pred_label_idx]
                    
                    title = f"True: {class_name} | Pred: {pred_class}\nFile: {file_path.name} (Clip {clip_idx+1})"
                    print(f"❌ 오답 발견: {file_path.name} (클립 {clip_idx+1}) -> '{pred_class}'로 잘못 예측함.")
                    
                    # 틀린 MFCC 이미지를 폴더에 저장
                    save_name = error_dir / f"error_{error_count:03d}_{class_name}_as_{pred_class}.png"
                    plot_and_save_mfcc(mfcc, title, save_path=save_name)

    print(f"\n✅ 분석 완료! 총 {error_count}개의 오답이 발견되었습니다.")
    print(f"📁 틀린 데이터의 시각화 이미지가 '{error_dir.absolute()}' 폴더에 저장되었습니다.")

if __name__ == "__main__":
    main()
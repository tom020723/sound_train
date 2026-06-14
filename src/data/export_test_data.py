from __future__ import annotations
from pathlib import Path
import numpy as np
from src.common.config import load_config
from src.features.librosa_mfcc import load_audio, split_fixed_clips

def get_raw_audio_sequence(file_path: Path, config, start_sec: int, count: int) -> list[np.ndarray]:
    """
    오디오 파일을 읽어 지정된 구간의 클립들을 C언어 short(int16_t) 범위의 정수 데이터로 변환합니다.
    """
    audio = load_audio(file_path, config)
    clips = split_fixed_clips(audio, config.clip_samples)

    target_clips = clips[start_sec : start_sec + count]
    
    raw_sequence_data = []
    for clip in target_clips:
        # librosa가 읽은 -1.0 ~ 1.0 사이의 float32 데이터를 C언어 int16_t 범위(-32768 ~ 32767)로 복원
        # 소수점이 끊기지 않도록 반올림(round) 후 int16 타입 변환
        raw_pcm = np.clip(np.round(clip * 32767.0), -32768, 32767).astype(np.int16)
        raw_sequence_data.append(raw_pcm)
        
    return raw_sequence_data

def main() -> None:
    config = load_config()
    
    # 가상의 마이크 입력 역할을 할 초(Second) 수 정의
    num_seconds = 2
    
    # 1초당 샘플 수 자동 계산 (예: 16000Hz 환경이면 16000개)
    samples_per_second = config.clip_samples 
    print(f"🎵 설정된 오디오 Format: Sampling Rate = {config.sample_rate}Hz, 1초당 샘플 수 = {samples_per_second}개")

    # =======================================================================
    # 📌 [폴더 방식 자동화] test_data/rain 및 test_data/not_rain 폴더 탐색
    # =======================================================================
    PROJECT_ROOT = Path(__file__).resolve().parent.parent.parent 
    
    rain_dir = PROJECT_ROOT / "drive" / "test_data" / "rain"
    not_rain_dir = PROJECT_ROOT / "drive" / "test_data" / "not_rain"
    
    # 폴더가 없으면 생성
    for d in [rain_dir, not_rain_dir]:
        if not d.exists():
            d.mkdir(parents=True, exist_ok=True)
            print(f"📁 새로 생성됨: {d}")

    def get_first_audio_file(folder: Path):
        for f in folder.iterdir():
            if f.is_file() and f.suffix.lower() in ['.wav', '.mp3', '.flac', '.aiff', '.aif']:
                return f
        return None

    rain_path = get_first_audio_file(rain_dir)
    not_rain_path = get_first_audio_file(not_rain_dir)

    if not rain_path:
        raise FileNotFoundError("🚨 'test_data/rain' 폴더에 오디오 파일이 없습니다. 비 오는 소리 파일을 넣어주세요!")
    if not not_rain_path:
        raise FileNotFoundError("🚨 'test_data/not_rain' 폴더에 오디오 파일이 없습니다. 맑은 날 소리 파일을 넣어주세요!")

    print("-" * 50)
    print(f"🎙️ 선택된 Rain 데이터     : {rain_path}")
    print(f"🎙️ 선택된 Not Rain 데이터 : {not_rain_path}")
    print("-" * 50)

    print("🔄 날것의 오디오 데이터(PCM) 추출 및 audio_data.h 변환 중...")
    
    # 비 오는 소리와 맑은 소리에서 각각 지정된 구간의 오디오 raw 파형 데이터를 추출
    rain_audio_seq = get_raw_audio_sequence(rain_path, config, start_sec=103, count=num_seconds)
    not_rain_audio_seq = get_raw_audio_sequence(not_rain_path, config, start_sec=145, count=num_seconds)

    # =======================================================================
    # 📌 C언어 헤더 파일(audio_data.h) 생성
    # =======================================================================
    header_filename = "audio_data.h"
    with open(header_filename, "w") as f:
        f.write("#ifndef AUDIO_DATA_H\n")
        f.write("#define AUDIO_DATA_H\n\n")
        
        f.write(f"#define AUDIO_NUM_SECONDS {num_seconds}\n")
        f.write(f"#define AUDIO_SAMPLES_PER_SEC {samples_per_second}\n\n")
        
        # 1. Rain Audio Data Array 
        f.write(f"// Rain Audio: {num_seconds} seconds (Shape: [{num_seconds}][{samples_per_second}])\n")
        f.write(f"const short test_rain_audio[{num_seconds}][{samples_per_second}] = {{\n")
        for pcm in rain_audio_seq:
            f.write("    {")
            # 16000개의 숫자를 한 줄에 콤마로 연결해서 출력
            f.write(",".join(map(str, pcm)))
            f.write("},\n")
        f.write("};\n\n")
        
        # 2. Not Rain Audio Data Array
        f.write(f"// Not Rain Audio: {num_seconds} seconds (Shape: [{num_seconds}][{samples_per_second}])\n")
        f.write(f"const short test_not_rain_audio[{num_seconds}][{samples_per_second}] = {{\n")
        for pcm in not_rain_audio_seq:
            f.write("    {")
            f.write(",".join(map(str, pcm)))
            f.write("},\n")
        f.write("};\n\n")
        
        f.write("#endif // AUDIO_DATA_H\n")
        
    print(f"✅ {header_filename} 파일이 성공적으로 생성되었습니다!")

if __name__ == "__main__":
    main()
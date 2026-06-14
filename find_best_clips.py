"""
rain/not_rain 테스트 오디오에서 모델이 가장 확실하게 분류하는 연속 2초 구간을 찾아
export_test_data.py의 start_sec를 자동으로 업데이트합니다.
"""
import re
import numpy as np
import tensorflow as tf
from pathlib import Path
from src.common.config import load_config
from src.features.cmsis_mfcc import build_mfcc_params, load_cmsis_mfcc_config, mfcc as cmsis_mfcc
from src.features.librosa_mfcc import load_audio, split_fixed_clips

config = load_config()
params = build_mfcc_params(load_cmsis_mfcc_config('configs/cmsis_mfcc.json'))
interp = tf.lite.Interpreter(model_path='models/rain_detector_int8.tflite')
interp.allocate_tensors()
inp_d  = interp.get_input_details()[0]
out_d  = interp.get_output_details()[0]
scale  = inp_d['quantization'][0]
zp     = inp_d['quantization'][1]

def infer_all(audio_file: Path):
    audio = load_audio(audio_file, config)
    clips = split_fixed_clips(audio, config.clip_samples)
    scores = []
    for clip in clips:
        feat = cmsis_mfcc(clip.astype('float32'), params)
        q    = np.round(feat / scale + zp).clip(-128, 127).astype(np.int8)
        interp.set_tensor(inp_d['index'], q[None, :, :, None])
        interp.invoke()
        r = interp.get_tensor(out_d['index'])[0]
        scores.append((int(r[0]), int(r[1])))  # (not_rain_raw, rain_raw)
    return scores

def find_best_pair(scores, want_rain: bool, count: int = 2):
    """want_rain=True: Rain 점수 합 최대, False: Not Rain 점수 합 최대"""
    best_score = -9999
    best_start = 0
    for i in range(len(scores) - count + 1):
        pair = scores[i : i + count]
        if want_rain:
            # 두 클립 모두 Rain으로 분류되어야 함
            if not all(ra > nr for nr, ra in pair):
                continue
            total = sum(ra for nr, ra in pair)
        else:
            # 두 클립 모두 Not Rain으로 분류되어야 함
            if not all(nr > ra for nr, ra in pair):
                continue
            total = sum(nr for nr, ra in pair)
        if total > best_score:
            best_score = total
            best_start = i
    return best_start, best_score

def get_audio_file(folder: Path):
    for f in folder.iterdir():
        if f.is_file() and f.suffix.lower() in ['.wav', '.mp3', '.flac', '.aiff', '.aif']:
            return f
    raise FileNotFoundError(f"{folder}에 오디오 파일이 없습니다.")

PROJECT_ROOT  = Path(__file__).resolve().parent
rain_file     = get_audio_file(PROJECT_ROOT / 'drive' / 'test_data' / 'rain')
not_rain_file = get_audio_file(PROJECT_ROOT / 'drive' / 'test_data' / 'not_rain')

print(f"Rain    파일: {rain_file.name}")
print(f"Not Rain파일: {not_rain_file.name}")
print("스캔 중...\n")

rain_scores     = infer_all(rain_file)
not_rain_scores = infer_all(not_rain_file)

rain_start,     rain_best     = find_best_pair(rain_scores,     want_rain=True)
not_rain_start, not_rain_best = find_best_pair(not_rain_scores, want_rain=False)

def pct(nr, ra, want_rain):
    raw = ra if want_rain else nr
    return int(((raw + 128) * 100) / 255)

print("=== Rain 최적 구간 ===")
for i in range(2):
    nr, ra = rain_scores[rain_start + i]
    print(f"  sec {rain_start + i:3d}: Rain {pct(nr,ra,True):3d}%  Not Rain {pct(nr,ra,False):3d}%")

print("\n=== Not Rain 최적 구간 ===")
for i in range(2):
    nr, ra = not_rain_scores[not_rain_start + i]
    print(f"  sec {not_rain_start + i:3d}: Not Rain {pct(nr,ra,False):3d}%  Rain {pct(nr,ra,True):3d}%")

# export_test_data.py 자동 업데이트
export_path = PROJECT_ROOT / 'src' / 'data' / 'export_test_data.py'
src = export_path.read_text(encoding='utf-8')

src = re.sub(
    r'(get_raw_audio_sequence\(rain_path,\s*config,\s*start_sec=)\d+',
    lambda m: m.group(1) + str(rain_start),
    src
)
src = re.sub(
    r'(get_raw_audio_sequence\(not_rain_path,\s*config,\s*start_sec=)\d+',
    lambda m: m.group(1) + str(not_rain_start),
    src
)
export_path.write_text(src, encoding='utf-8')

print(f"\nexport_test_data.py 업데이트 완료:")
print(f"  rain     start_sec = {rain_start}")
print(f"  not_rain start_sec = {not_rain_start}")
print(f"\n다음 명령어로 audio_data.h를 재생성하세요:")
print(f"  python -m src.data.export_test_data")

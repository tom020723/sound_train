"""
not_rain 테스트 오디오에서 TFLite 모델이 가장 확실하게 Not Rain으로 분류하는
연속 2초 구간을 찾아줍니다.
"""
import numpy as np
import tensorflow as tf
from pathlib import Path
from src.common.config import load_config
from src.features.cmsis_mfcc import build_mfcc_params, load_cmsis_mfcc_config, mfcc as cmsis_mfcc
from src.features.librosa_mfcc import load_audio, split_fixed_clips

config      = load_config()
params      = build_mfcc_params(load_cmsis_mfcc_config('configs/cmsis_mfcc.json'))
interp      = tf.lite.Interpreter(model_path='models/rain_detector_int8.tflite')
interp.allocate_tensors()
inp_d       = interp.get_input_details()[0]
out_d       = interp.get_output_details()[0]
scale       = inp_d['quantization'][0]
zp          = inp_d['quantization'][1]

def infer(clip_float):
    feat = cmsis_mfcc(clip_float.astype('float32'), params)
    q    = np.round(feat / scale + zp).clip(-128, 127).astype(np.int8)
    interp.set_tensor(inp_d['index'], q[None, :, :, None])
    interp.invoke()
    r   = interp.get_tensor(out_d['index'])[0]
    nr, ra = int(r[0]), int(r[1])
    # Not Rain 확률 (0~100%)
    nr_pct = int(((nr + 128) * 100) / 255)
    return nr, ra, nr_pct

not_rain_dir = Path('drive/test_data/not_rain')
audio_file   = next(f for f in not_rain_dir.iterdir()
                    if f.suffix.lower() in ['.wav', '.mp3', '.flac', '.aiff', '.aif'])

print(f"파일: {audio_file.name}")
audio = load_audio(audio_file, config)
clips = split_fixed_clips(audio, config.clip_samples)
print(f"전체 클립 수: {len(clips)} 초\n")

print(f"{'초':>4}  {'NR raw':>6}  {'R raw':>6}  {'Not Rain%':>9}  판정")
print("-" * 45)

results = []
for i, clip in enumerate(clips):
    nr, ra, nr_pct = infer(clip)
    verdict = 'Not Rain' if nr > ra else 'Rain    '
    print(f"{i:>4}  {nr:>6}  {ra:>6}  {nr_pct:>8}%  {verdict}")
    results.append((i, nr, ra, nr_pct))

# 연속 2초 구간 중 두 클립의 Not Rain% 합이 가장 높은 구간 찾기
print("\n=== 연속 2초 구간 Not Rain% 합산 TOP 5 ===")
pairs = []
for i in range(len(results) - 1):
    s0 = results[i][3]
    s1 = results[i+1][3]
    both_not_rain = results[i][2] < results[i][1] and results[i+1][2] < results[i+1][1]
    pairs.append((s0 + s1, i, s0, s1, both_not_rain))

pairs.sort(reverse=True)
for total, start, s0, s1, both in pairs[:5]:
    tag = " ← 둘 다 Not Rain" if both else ""
    print(f"  start_sec={start:3d}  ({s0}% + {s1}% = {total}%){tag}")

best_both = next(((t, s, a, b) for t, s, a, b, ok in pairs if ok), None)
if best_both:
    print(f"\n추천: start_sec={best_both[1]}  ({best_both[2]}% / {best_both[3]}%)")
    print(f"export_test_data.py 72번째 줄을 이렇게 바꾸세요:")
    print(f"    not_rain_audio_seq = get_raw_audio_sequence(not_rain_path, config, start_sec={best_both[1]}, count=num_seconds)")
else:
    print("\n경고: 연속 2초 모두 Not Rain인 구간이 없습니다. 테스트 오디오 파일을 교체하세요.")

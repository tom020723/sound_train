import numpy as np
import tensorflow as tf
from pathlib import Path
from src.common.config import load_config
from src.features.cmsis_mfcc import build_mfcc_params, load_cmsis_mfcc_config, mfcc as cmsis_mfcc
from src.features.librosa_mfcc import load_audio, split_fixed_clips

config = load_config()
cmsis_params = build_mfcc_params(load_cmsis_mfcc_config('configs/cmsis_mfcc.json'))
AI_SCALE, AI_ZP = 0.33135787, 85

keras_model = tf.keras.models.load_model('models/rain_detector.keras')

interp = tf.lite.Interpreter(model_path='models/rain_detector_int8.tflite')
interp.allocate_tensors()
inp_d = interp.get_input_details()[0]
out_d = interp.get_output_details()[0]

def run_keras(feat):
    pred = keras_model.predict(feat[None,:,:,None], verbose=0)[0]
    return pred[0], pred[1]

def run_tflite(feat):
    inp_tensor = feat[None, :, :, None].astype(inp_d['dtype'])
    interp.set_tensor(inp_d['index'], inp_tensor)
    interp.invoke()
    r = interp.get_tensor(out_d['index'])[0]
    return r[0], r[1]

def label(nr, r):
    return "Rain" if r > nr else "Not Rain"

rain_files     = [f for f in Path('data/raw/rain').iterdir() if f.suffix == '.wav'][:3]
not_rain_files = [f for f in Path('data/raw/not_rain').iterdir() if f.suffix == '.wav'][:3]

print("=== Float Keras model ===")
for lbl, files in [('rain', rain_files), ('not_rain', not_rain_files)]:
    for fpath in files:
        audio = load_audio(fpath, config)
        clips = split_fixed_clips(audio, config.clip_samples)
        if not clips:
            continue
        feat = cmsis_mfcc(clips[0].astype('float32'), cmsis_params)
        nr, r = run_keras(feat)
        print(f"  [{lbl}] {fpath.name[:40]:40s}  not_rain={nr:.3f}  rain={r:.3f}  => {label(nr, r)}")

print()
print("=== TFLite int8 model ===")
for lbl, files in [('rain', rain_files), ('not_rain', not_rain_files)]:
    for fpath in files:
        audio = load_audio(fpath, config)
        clips = split_fixed_clips(audio, config.clip_samples)
        if not clips:
            continue
        feat = cmsis_mfcc(clips[0].astype('float32'), cmsis_params)
        nr, r = run_tflite(feat)
        print(f"  [{lbl}] {fpath.name[:40]:40s}  not_rain={nr:.3f}  rain={r:.3f}  => {label(nr, r)}")

# 테스트 클립도 같이 확인
print()
print("=== Test clips (audio_data.h 와 동일) ===")
rain_test     = list(Path('drive/test_data/rain').iterdir())[0]
not_rain_test = list(Path('drive/test_data/not_rain').iterdir())[0]
rain_audio    = load_audio(rain_test, config)
nr_audio      = load_audio(not_rain_test, config)
rain_clips    = split_fixed_clips(rain_audio, config.clip_samples)
nr_clips      = split_fixed_clips(nr_audio,   config.clip_samples)

for lbl, clips, start in [('rain', rain_clips, 76), ('not_rain', nr_clips, 20)]:
    feat = cmsis_mfcc(clips[start].astype('float32'), cmsis_params)
    nr_k, r_k = run_keras(feat)
    nr_t, r_t = run_tflite(feat)
    print(f"  [{lbl}] Keras: not_rain={nr_k:.3f} rain={r_k:.3f} => {label(nr_k, r_k)}")
    print(f"  [{lbl}] TFLite: not_rain={nr_t:4d}  rain={r_t:4d}   => {label(nr_t, r_t)}")

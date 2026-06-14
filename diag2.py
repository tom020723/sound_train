import numpy as np
import tensorflow as tf
from pathlib import Path
from src.common.config import load_config
from src.features.cmsis_mfcc import build_mfcc_params, load_cmsis_mfcc_config, mfcc as cmsis_mfcc
from src.features.librosa_mfcc import load_audio, split_fixed_clips

config = load_config()
cmsis_params = build_mfcc_params(load_cmsis_mfcc_config('configs/cmsis_mfcc.json'))

interp = tf.lite.Interpreter(model_path='models/rain_detector_int8.tflite')
interp.allocate_tensors()
inp_d = interp.get_input_details()[0]
out_d = interp.get_output_details()[0]
scale = inp_d['quantization'][0]
zp    = inp_d['quantization'][1]
print('Input  dtype:', inp_d['dtype'], 'scale:', scale, 'zp:', zp)
print('Output dtype:', out_d['dtype'], 'scale:', out_d['quantization'][0], 'zp:', out_d['quantization'][1])

def infer(clip_float):
    feat = cmsis_mfcc(clip_float.astype('float32'), cmsis_params)
    q = np.round(feat / scale + zp).clip(-128, 127).astype(np.int8)
    interp.set_tensor(inp_d['index'], q[None, :, :, None])
    interp.invoke()
    r = interp.get_tensor(out_d['index'])[0]
    nr, ra = int(r[0]), int(r[1])
    verdict = 'Rain' if ra > nr else 'Not Rain'
    return nr, ra, verdict

rain_files     = [f for f in Path('data/raw/rain').iterdir() if f.suffix == '.wav'][:2]
not_rain_files = [f for f in Path('data/raw/not_rain').iterdir() if f.suffix == '.wav'][:2]

print()
print('== Training data clips ==')
for lbl, files in [('rain', rain_files), ('not_rain', not_rain_files)]:
    for fpath in files:
        audio = load_audio(fpath, config)
        clips = split_fixed_clips(audio, config.clip_samples)
        if not clips:
            continue
        nr, ra, v = infer(clips[0])
        print(f'  [{lbl}] {fpath.name[:40]:40s}  nr={nr:4d} r={ra:4d} => {v}')

print()
print('== Test audio (same as audio_data.h) ==')
rain_test  = list(Path('drive/test_data/rain').iterdir())[0]
nr_test    = list(Path('drive/test_data/not_rain').iterdir())[0]
rain_audio = load_audio(rain_test, config)
nr_audio   = load_audio(nr_test, config)
rain_clips = split_fixed_clips(rain_audio, config.clip_samples)
nr_clips   = split_fixed_clips(nr_audio, config.clip_samples)

for lbl, clips, start in [('rain', rain_clips, 76), ('not_rain', nr_clips, 20)]:
    nr, ra, v = infer(clips[start])
    print(f'  [{lbl}] nr={nr:4d} r={ra:4d} => {v}')

import numpy as np
import tensorflow as tf
import matplotlib.pyplot as plt
import librosa
from pathlib import Path

from src.common.config import load_config
from src.data.audio_files import iter_audio_files
from src.features.cmsis_mfcc import build_mfcc_params, load_cmsis_mfcc_config, mfcc as cmsis_mfcc
from src.features.librosa_mfcc import load_audio, split_fixed_clips

def plot_and_save_mfcc(mfcc_data, title, save_path):
    plt.figure(figsize=(10, 4))
    librosa.display.specshow(mfcc_data.T, x_axis='time')
    plt.colorbar(format='%+2.0f dB')
    plt.title(title)
    plt.tight_layout()
    plt.savefig(save_path)
    plt.close()

def main():
    config = load_config()
    cmsis_params = build_mfcc_params(load_cmsis_mfcc_config("configs/cmsis_mfcc.json"))
    model = tf.keras.models.load_model(config.model_dir / "rain_detector.keras")

    error_dir = Path("error_analysis")
    error_dir.mkdir(exist_ok=True)

    error_count = 0

    for true_label_idx, class_name in enumerate(config.classes):
        files = iter_audio_files(config.data_dir / class_name)
        
        for file_path in files:
            try:
                audio = load_audio(file_path, config)
            except Exception:
                continue
            
            clips = list(split_fixed_clips(audio, config.clip_samples))
            
            for clip_idx, clip in enumerate(clips):
                mfcc = cmsis_mfcc(clip.astype(np.float32), cmsis_params)
                
                pred_probs = model.predict(mfcc[None, ..., None], verbose=0)
                pred_label_idx = pred_probs.argmax(axis=1)[0]
                
                if pred_label_idx != true_label_idx:
                    error_count += 1
                    pred_class = config.classes[pred_label_idx]
                    
                    title = f"True: {class_name} | Pred: {pred_class}\nFile: {file_path.name} (Clip {clip_idx+1})"
                    
                    save_name = error_dir / f"error_{error_count:03d}_{class_name}_as_{pred_class}.png"
                    plot_and_save_mfcc(mfcc, title, save_path=save_name)

if __name__ == "__main__":
    main()

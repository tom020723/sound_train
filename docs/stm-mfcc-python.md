# STM MFCC Python 검증 도구

이 문서는 STM32 CMSIS-DSP MFCC와 Python MFCC를 맞추기 위한 Python 쪽 도구 사용법을 정리합니다.

## 목적

STM 보드에서 `arm_mfcc_f32`로 계산한 MFCC가 Python에서 계산한 MFCC와 같은지 확인하려면 같은 PCM 입력에 대한 결과를 숫자로 비교해야 합니다.

이번 브랜치에서는 먼저 Python 쪽에 아래 기능을 추가했습니다.

- CMSIS-DSP에 가까운 MFCC 계수 생성
- Python 기준 MFCC 계산
- STM에 넣을 수 있는 `mfcc_params.h` 생성
- golden PCM/MFCC 생성
- STM 출력 CSV와 Python MFCC CSV 비교

## 설정

MFCC 설정은 `configs/cmsis_mfcc.json`에서 관리합니다.

현재 기본값:

```text
sample_rate: 16000
frame_length: 480
frame_step: 160
fft_length: 512
num_mel_filters: 26
num_mfcc: 13
window_type: hamming
```

이 설정은 STM 쪽 `arm_mfcc_init_f32`에 들어갈 계수와 맞춰야 합니다.

## Golden 데이터 생성

```powershell
python -m src.tools.generate_mfcc_golden
```

생성 위치:

```text
artifacts/mfcc_golden/
  input_pcm_i16.raw
  input_pcm_i16.csv
  python_mfcc.csv
  mfcc_params.h
```

현재 golden 입력은 1초, 16 kHz, int16 PCM입니다.

현재 MFCC shape:

```text
(98, 13)
```

98 frame이 나오는 이유는 CMSIS-DSP 스타일로 center padding 없이 30 ms frame, 10 ms step을 사용하기 때문입니다.

## MFCC 파라미터 헤더만 export

```powershell
python -m src.tools.export_mfcc_params --output artifacts/mfcc_params.h
```

나중에 STM 프로젝트에 넣을 때는 출력 경로를 바꿔 사용할 수 있습니다.

```powershell
python -m src.tools.export_mfcc_params --output Stm32/Core/Inc/mfcc_params.h
```

## STM 결과와 비교

STM에서 같은 `input_pcm_i16.raw` 또는 동일한 C array를 입력으로 사용해 MFCC를 계산하고, 결과를 CSV로 저장합니다.

예상 형식:

```text
frame0_mfcc0,frame0_mfcc1,...,frame0_mfcc12
frame1_mfcc0,frame1_mfcc1,...,frame1_mfcc12
...
```

비교 명령:

```powershell
python -m src.tools.compare_mfcc artifacts/mfcc_golden/python_mfcc.csv stm_mfcc.csv
```

출력 지표:

- `max_abs_error`
- `mean_abs_error`
- `rmse`
- `cosine_similarity`

처음에는 `arm_mfcc_f32` 기준으로 맞추는 것을 목표로 합니다. `q15`나 `q31`로 내려가는 것은 f32 결과가 맞은 뒤에 비교하는 편이 안전합니다.

## 주의

현재 Python 구현은 CMSIS-DSP와 비교하기 위한 기준 구현입니다. 실제 STM 결과와 1차 비교를 해봐야 window, filterbank, log, DCT 스케일이 완전히 맞는지 판단할 수 있습니다.

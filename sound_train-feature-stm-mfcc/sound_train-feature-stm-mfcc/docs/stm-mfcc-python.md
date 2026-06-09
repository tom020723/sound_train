# STM32 CMSIS-DSP MFCC 검증 정리

이 문서는 Python에서 만든 MFCC 전처리와 STM32 보드에서 실행한 CMSIS-DSP `arm_mfcc_f32()` 결과를 맞춘 과정을 정리합니다.

## 목표

비 소리 감지 모델을 STM32 TinyML 환경에서 사용하려면 학습 때 쓰는 MFCC와 보드에서 계산하는 MFCC가 같은 방식이어야 합니다.

이번 작업의 목표는 다음과 같습니다.

- Python에서 CMSIS-DSP와 같은 MFCC 파라미터 생성
- golden PCM 입력과 Python MFCC 출력 생성
- STM32F411RETx 보드에서 같은 PCM 입력으로 `arm_mfcc_f32()` 실행
- UART 출력 결과를 Python golden 결과와 비교

## 사용한 MFCC 설정

설정 파일은 `configs/cmsis_mfcc.json`입니다.

```text
sample_rate: 16000
frame_length: 480
frame_step: 160
fft_length: 512
num_mel_filters: 26
num_mfcc: 13
window_type: hamming
```

1초짜리 16 kHz PCM 입력을 사용하면 30 ms frame, 10 ms step 기준으로 총 98개 프레임이 생성됩니다.

## STM32 쪽 구성

STM32Cube F4 기본 패키지에 포함된 CMSIS-DSP는 오래된 버전이라 `arm_mfcc_f32()`가 없었습니다. 그래서 최신 CMSIS-DSP를 `Stm32/Middlewares/CMSIS-DSP`에 두고, Debug 빌드 설정에서 다음 include path를 사용하도록 맞췄습니다.

```text
../Middlewares/CMSIS-DSP/Include
../Middlewares/CMSIS-DSP/PrivateInclude
```

보드 검증용으로 필요한 CMSIS-DSP 소스 파일은 `Stm32/Core/Src/CMSIS_DSP_MFCC` 아래에 추가했습니다.

STM에서는 다음 파일들을 사용합니다.

- `Stm32/Core/Inc/mfcc_params.h`: DCT, mel filter, window 계수
- `Stm32/Core/Inc/mfcc_golden_pcm.h`: golden PCM 입력
- `Stm32/Core/Src/mfcc_debug.c`: MFCC 계산 후 UART CSV 출력

## Python 구현 수정

처음 Python 구현은 일반적인 MFCC 방식에 가깝게 power spectrum을 사용했습니다.

```text
abs(FFT)^2 -> mel filter -> log(max(x, floor)) -> DCT
```

하지만 CMSIS-DSP `arm_mfcc_f32()`는 다음 흐름을 사용합니다.

```text
frame absmax normalize
-> window
-> FFT magnitude
-> normalize scale 복원
-> mel filter
-> log(x + 1e-6)
-> DCT
```

그래서 `src/features/cmsis_mfcc.py`를 CMSIS-DSP 흐름에 맞게 수정했습니다. 이 차이를 맞추기 전에는 cosine similarity는 높았지만 절대 오차가 컸고, 수정 후에는 보드 출력과 거의 동일하게 맞았습니다.

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

STM용 PCM 헤더는 다음 명령으로 만들 수 있습니다.

```powershell
python -m src.tools.export_pcm_header --output Stm32/Core/Inc/mfcc_golden_pcm.h
```

MFCC 파라미터 헤더는 다음 명령으로 만들 수 있습니다.

```powershell
python -m src.tools.export_mfcc_params --output Stm32/Core/Inc/mfcc_params.h
```

## 보드 출력 결과

보드에서 UART로 받은 결과는 [result.txt](../result.txt)에 저장했습니다.

출력은 다음 형태입니다.

```text
MFCC_BEGIN
<98 rows of 13 MFCC values>
MFCC_END 98
```

`MFCC_END 98`이 찍혔으므로 STM32에서 98개 프레임을 모두 처리한 것이 확인됩니다.

## 비교 결과

비교 명령:

```powershell
python -m src.tools.compare_mfcc artifacts/mfcc_golden/python_mfcc.csv result.txt
```

결과:

```text
shape: (98, 13)
max_abs_error: 8.344650269e-06
mean_abs_error: 1.710589572e-06
rmse: 2.168050742e-06
cosine_similarity: 0.999999940
```

이 정도 오차는 float 연산과 출력 소수점 자릿수 차이 수준입니다. 따라서 현재 Python MFCC와 STM32 CMSIS-DSP MFCC는 같은 전처리로 맞춰진 상태라고 볼 수 있습니다.

## 테스트

Python 테스트 결과:

```text
4 passed
```

## 다음 단계

현재는 f32 MFCC 검증 단계입니다. 이후 모델 입력을 TinyML용 int8로 사용할 때는 f32 MFCC 결과에 대해 학습/추론에서 같은 quantization scale과 zero point를 적용해야 합니다. q15/q31 MFCC로 내려가는 것은 f32 기준 검증이 끝난 뒤에 비교하는 것이 안전합니다.

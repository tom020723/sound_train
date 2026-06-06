# sound_train

TinyML용 비 소리 감지 모델 프로토타입입니다.

1차 목표는 아래 두 클래스를 구분하는 이진 분류 모델입니다.

- `rain`
- `not_rain`

오디오를 MFCC 특징으로 변환하고, 작은 모델을 학습한 뒤 임베디드 추론에 사용할 수 있도록 TensorFlow Lite int8 모델로 내보냅니다.

## 개발 환경 설정

Python 3.13 가상환경을 만들고 활성화합니다.

```powershell
py -3.13 -m venv .venv
.\.venv\Scripts\Activate.ps1
python -m pip install -r requirements.txt
```

Microsoft Store Python에서 Temp 폴더 권한 문제로 pip 설치가 실패하면, 프로젝트 안의 임시 폴더를 사용합니다.

```powershell
New-Item -ItemType Directory -Force .tmp
$env:TEMP = "$PWD\.tmp"
$env:TMP = "$PWD\.tmp"
.\.venv\Scripts\python.exe -m ensurepip --upgrade
```

## 데이터 구조

원본 다운로드 데이터는 `drive/`에 그대로 둡니다. 이 폴더는 git에 올리지 않습니다.

```text
drive/
  rain/
  ambient noise/
```

학습에 사용할 데이터는 `data/raw/` 아래에 정리된 사본으로 둡니다.

```text
data/
  raw/
    rain/
    not_rain/
  processed/
```

현재 매핑은 다음과 같습니다.

- `drive/rain` -> `data/raw/rain`
- `drive/ambient noise` -> `data/raw/not_rain`

정리된 오디오 파일과 `data/raw/manifest.csv`도 git에는 올리지 않습니다. 저장소에는 폴더 구조를 유지하기 위한 `.gitkeep`만 포함합니다.

## 🚀 데이터 정리 및 모델 학습 가이드

### 1. 데이터 정리 및 자동 밸런싱 (Data Curation)
`drive/`의 원본 데이터를 `data/raw/`로 복사합니다. 이때 `rain`과 `not_rain` 데이터의 균형(각각 1000개 클립)을 맞추기 위한 **자동 다운샘플링(필터링)**이 함께 수행됩니다.

```powershell
python -m src.data.curate_raw_data --clear

# 1. 구글 드라이브 원본 복사 및 ESC-50 데이터 자동 필터링 (클래스당 32개)
python -m src.data.curate_raw_data --clear

# 2. 오디오 파일 1초 단위 슬라이싱 및 MFCC 특징 추출
python -m src.data.prepare_dataset

# 3. [검증] 최종 데이터셋이 1000 vs 1000으로 예쁘게 맞춰졌는지 개수 확인
python -m src.data.check_counts

# 4. 딥러닝 모델 학습 시작 (30 Epochs 진행 및 models/rain_detector.keras 저장)
python -m src.training.train

# 5. 테스트 데이터셋 기반 모델 최종 성능 평가 (정확도 및 Confusion Matrix 출력)
python -m src.training.evaluate

# 6. 학습/테스트 데이터 전체를 대상으로 오답 분석 (error_analysis/ 폴더에 시각화 저장)
python -m src.training.analyze_errors

## 학습 흐름

데이터 준비부터 int8 TFLite 변환까지의 기본 흐름입니다.

```powershell
python -m src.data.prepare_dataset
python -m src.training.train
python -m src.training.evaluate
python -m src.training.export_tflite
python -m src.inference.predict path\to\audio.wav
```

각 스크립트의 역할은 다음과 같습니다.

- `src.data.curate_raw_data`: `drive/` 원본 데이터를 `data/raw/` 학습 구조로 정리
- `src.data.prepare_dataset`: 오디오 파일을 읽고 1초 단위 MFCC 데이터셋 생성
- `src.training.train`: MFCC 데이터셋으로 작은 CNN 계열 모델 학습
- `src.training.evaluate`: 테스트셋 성능 평가
- `src.training.export_tflite`: representative dataset 기반 full int8 TFLite 변환
- `src.inference.predict`: 새 오디오 파일의 `rain` / `not_rain` 예측

## 코드 구조

`src/`는 역할별 패키지로 나눕니다.

```text
src/
  common/      설정과 공통 유틸리티
  data/        원본 데이터 정리와 데이터셋 생성
  features/    MFCC 등 특징 추출 구현
  models/      모델 아키텍처
  training/    학습, 평가, TFLite 변환
  inference/   예측 실행 코드
```

## TinyML 메모

현재 기준 입력 파이프라인은 다음과 같습니다.

```text
16 kHz mono 오디오
-> 1초 고정 길이 클립
-> MFCC 추출
-> 학습 데이터 기준 mean/std 정규화
-> TensorFlow Lite int8 양자화
```

기본 설정은 `configs/default.json`에서 관리합니다.

현재 기본값:

- sample rate: `16000`
- clip length: `1초`
- MFCC 개수: `13`
- 클래스: `not_rain`, `rain`

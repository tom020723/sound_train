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

```text
data/
  raw/
    rain/
    not_rain/
  processed/
models/
src/
```

원본 오디오 파일은 아래 위치에 넣습니다.

- 비 소리: `data/raw/rain`
- 비가 아닌 소리: `data/raw/not_rain`

지원 예정 확장자는 `.wav`, `.mp3`, `.flac`, `.ogg`, `.m4a`입니다.

## 실행 흐름

데이터 준비부터 int8 TFLite 변환까지의 기본 흐름입니다.

```powershell
python -m src.prepare_dataset
python -m src.train
python -m src.evaluate
python -m src.export_tflite
python -m src.predict path\to\audio.wav
```

각 스크립트의 역할은 다음과 같습니다.

- `src.prepare_dataset`: 오디오 파일을 읽고 1초 단위 MFCC 데이터셋 생성
- `src.train`: MFCC 데이터셋으로 작은 CNN 계열 모델 학습
- `src.evaluate`: 테스트셋 성능 평가
- `src.export_tflite`: representative dataset 기반 full int8 TFLite 변환
- `src.predict`: 새 오디오 파일의 `rain` / `not_rain` 예측

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

# 데이터 가공 설계

이 문서는 비 소리 감지 모델 학습에 사용할 오디오 데이터를 어떻게 정리하고, 어떤 기준으로 클립을 만들지 정리합니다.

## 목표

현재 목표는 `rain` / `not_rain` 이진 분류 모델입니다.

데이터 가공에서 가장 중요한 기준은 다음 두 가지입니다.

- 같은 원본 파일에서 나온 비슷한 클립이 train/test에 동시에 들어가지 않게 한다.
- TinyML 추론 환경에서 쓰기 쉬운 고정 길이 MFCC 입력을 만든다.

## 원본과 학습 데이터 분리

원본 다운로드 데이터는 `drive/`에 그대로 보관합니다.

```text
drive/
  rain/
  ambient noise/
```

학습에 사용할 정리본은 `data/raw/`에 둡니다.

```text
data/raw/
  rain/
  not_rain/
```

현재 매핑은 다음과 같습니다.

- `drive/rain` -> `data/raw/rain`
- `drive/ambient noise` -> `data/raw/not_rain`

정리 명령:

```powershell
python -m src.data.curate_raw_data --clear
```

`drive/`, `data/raw`의 실제 오디오, `data/raw/manifest.csv`, `data/processed`는 git에 올리지 않습니다.

## 데이터셋 생성 방식

데이터셋 생성은 `src.data.prepare_dataset`에서 처리합니다.

```powershell
python -m src.data.prepare_dataset
```

처리 순서:

1. 클래스별 원본 파일 목록 수집
2. 파일 단위로 train/validation/test split
3. 각 split 안에서 오디오 로드
4. 고정 길이 클립 생성
5. 너무 조용한 클립 제거
6. MFCC 추출
7. train split 기준 mean/std 계산
8. `data/processed/dataset.npz` 저장

파일 단위 split을 먼저 하는 이유는 데이터 누수를 줄이기 위해서입니다. 긴 원본 파일을 먼저 1초 클립으로 자른 뒤 랜덤 split하면 같은 원본에서 나온 거의 같은 소리가 train과 test에 동시에 들어갈 수 있습니다.

## 현재 클립 설정

기본 설정은 `configs/default.json`에서 관리합니다.

```json
{
  "sample_rate": 16000,
  "clip_seconds": 1.0,
  "stride_seconds": 0.5,
  "min_clip_rms": 0.001,
  "balance_train_classes": true
}
```

의미:

- `sample_rate`: 모든 오디오를 16 kHz mono로 읽습니다.
- `clip_seconds`: 모델 입력 단위는 1초입니다.
- `stride_seconds`: 0.5초 간격으로 겹치는 클립을 만듭니다.
- `min_clip_rms`: RMS가 너무 낮은 클립은 제외합니다.
- `balance_train_classes`: train split만 클래스별 clip 수를 맞춥니다.

validation/test split은 실제 분포를 유지합니다. 평가셋까지 강제로 균형화하면 실제 성능 해석이 흐려질 수 있기 때문입니다.

## 저장되는 데이터

`dataset.npz`에는 feature와 label뿐 아니라 추적용 메타데이터도 함께 저장합니다.

```text
x_train, y_train
train_source_paths
train_clip_start_seconds
train_clip_rms

x_val, y_val
val_source_paths
val_clip_start_seconds
val_clip_rms

x_test, y_test
test_source_paths
test_clip_start_seconds
test_clip_rms

mean, std
classes
```

이 메타데이터는 나중에 오탐/미탐을 분석할 때 어떤 원본 파일의 어느 구간이 문제였는지 추적하는 데 사용합니다.

## 현재 데이터셋 생성 결과

현재 수집된 데이터 기준 생성 결과:

```text
not_rain: 132 train files, 34 val files, 42 test files
rain: 57 train files, 15 val files, 18 test files

train: 5248 clips {'not_rain': 2624, 'rain': 2624}
val: 828 clips {'not_rain': 693, 'rain': 135}
test: 680 clips {'not_rain': 365, 'rain': 315}
```

MFCC feature shape:

```text
x_train: (5248, 101, 13)
x_val:   (828, 101, 13)
x_test:  (680, 101, 13)
```

## 다음 개선 후보

- hard negative 보강: 물 흐르는 소리, 샤워기, 분수, 에어컨, 선풍기
- thunderstorm 데이터 품질 확인: 천둥만 강한 파일은 별도 관리 가능
- RMS threshold 튜닝
- gain augmentation 추가
- background noise mix augmentation 추가
- CMSIS-DSP 기준 MFCC 구현 추가

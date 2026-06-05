# 학습 결과 기록

이 문서는 현재 데이터 가공 파이프라인으로 학습한 첫 번째 기준 모델 결과를 기록합니다.

## 실행 환경

- Python: `3.13.13`
- TensorFlow: `2.21.0`
- 모델: `src.models.cnn.build_model`
- 입력 feature: `MFCC (101, 13, 1)`
- 학습 데이터: `data/processed/dataset.npz`
- 저장 모델: `models/rain_detector.keras`
- 학습 이력: `models/training_history.json`

TensorFlow 2.11 이상 native Windows 환경에서는 GPU를 사용하지 않는다는 경고가 출력됩니다. 이번 학습도 CPU로 실행했습니다.

## 데이터셋

현재 데이터셋 생성 결과:

```text
train: 5248 clips {'not_rain': 2624, 'rain': 2624}
val: 828 clips {'not_rain': 693, 'rain': 135}
test: 680 clips {'not_rain': 365, 'rain': 315}
```

데이터셋은 파일 단위로 split한 뒤 각 split 안에서 1초 clip을 생성했습니다. train split만 클래스 균형화를 적용했습니다.

## 학습 결과

30 epoch 학습을 실행했습니다.

```text
best validation accuracy: 0.9106 at epoch 21
best validation loss: 0.2918 at epoch 21

final train accuracy: 0.9920
final train loss: 0.0230
final validation accuracy: 0.8080
final validation loss: 0.7089
```

최종 epoch 기준으로는 train 성능이 매우 높고 validation 성능이 흔들립니다. 현재 모델은 과적합 가능성이 있습니다.

## Test 평가

`python -m src.training.evaluate` 결과:

```text
confusion matrix:
[[339  26]
 [ 81 234]]

              precision    recall  f1-score   support

    not_rain       0.81      0.93      0.86       365
        rain       0.90      0.74      0.81       315

    accuracy                           0.84       680
   macro avg       0.85      0.84      0.84       680
weighted avg       0.85      0.84      0.84       680
```

해석:

- `not_rain` recall은 0.93으로 높습니다.
- `rain` precision은 0.90으로 높지만 recall은 0.74입니다.
- 즉, 비라고 판단한 경우는 꽤 믿을 만하지만 실제 비 소리 중 일부를 놓칩니다.

비 감지기 관점에서는 rain recall이 더 중요할 수 있으므로, 다음 단계에서는 놓친 rain clip을 분석해야 합니다.

## 다음 개선 후보

- validation accuracy가 가장 높았던 epoch의 모델을 저장하도록 checkpoint 추가
- early stopping 적용
- rain recall 개선을 위해 threshold 조정
- test false negative 원본 파일 분석
- thunderstorm 파일 중 비보다 천둥이 강한 구간 확인
- hard negative로 물소리 계열 추가
- augmentation 추가 전에 현재 오탐/미탐 목록부터 확인

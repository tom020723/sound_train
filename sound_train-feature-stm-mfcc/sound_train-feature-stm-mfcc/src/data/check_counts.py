from __future__ import annotations

import numpy as np
from src.common.config import load_config

def main() -> None:
    config = load_config()
    dataset_path = config.processed_dir / "dataset.npz"

    if not dataset_path.exists():
        print(f"❌ 데이터셋 파일을 찾을 수 없습니다: {dataset_path}")
        print("먼저 'python -m src.data.prepare_dataset'을 실행해주세요.")
        return

    # 데이터 불러오기
    data = np.load(dataset_path, allow_pickle=True)
    
    # 쪼개진 라벨(y) 데이터들 가져오기
    y_train = data["y_train"]
    y_val = data["y_val"]
    y_test = data["y_test"]

    # 전체 개수를 세기 위해 하나로 합치기
    y_all = np.concatenate([y_train, y_val, y_test])

    print("\n📊 생성된 최종 데이터셋 개수 확인 📊")
    print("=" * 35)
    
    total_count = 0
    for label_idx, class_name in enumerate(config.classes):
        # 해당 클래스의 개수 계산
        count = np.sum(y_all == label_idx)
        total_count += count
        print(f" 🎯 {class_name:<15} : {count} 개")
        
    print("-" * 35)
    print(f" 💡 총 합계        : {total_count} 개\n")

if __name__ == "__main__":
    main()
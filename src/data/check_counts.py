from __future__ import annotations

import numpy as np
from src.common.config import load_config


def main() -> None:
    config = load_config()
    dataset_path = config.processed_dir / "dataset.npz"

    if not dataset_path.exists():
        print(f"Dataset not found: {dataset_path}")
        print("Run 'python -m src.data.prepare_dataset' first.")
        return

    data = np.load(dataset_path, allow_pickle=True)

    y_train = data["y_train"]
    y_val = data["y_val"]
    y_test = data["y_test"]

    y_all = np.concatenate([y_train, y_val, y_test])

    print("\nDataset clip counts")
    print("=" * 35)

    total_count = 0
    for label_idx, class_name in enumerate(config.classes):
        count = np.sum(y_all == label_idx)
        total_count += count
        print(f"  {class_name:<15}: {count}")

    print("-" * 35)
    print(f"  Total          : {total_count}\n")


if __name__ == "__main__":
    main()

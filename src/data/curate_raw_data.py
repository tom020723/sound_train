from __future__ import annotations

import argparse
import csv
import random
import shutil
from collections import defaultdict
from pathlib import Path

from src.data.audio_files import iter_audio_files


DEFAULT_CLASS_SOURCES = {
    "rain": Path("drive/rain"),
    "not_rain": Path("drive/ambient noise"),
}


def safe_name(source_root: Path, audio_path: Path) -> str:
    relative = audio_path.relative_to(source_root)
    parts = [part.strip().replace(" ", "_") for part in relative.parts]
    return "__".join(parts)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--drive-dir", type=Path, default=Path("drive"))
    parser.add_argument("--data-dir", type=Path, default=Path("data/raw"))
    parser.add_argument("--clear", action="store_true")
    args = parser.parse_args()

    class_sources = {
        class_name: args.drive_dir / source.relative_to("drive")
        for class_name, source in DEFAULT_CLASS_SOURCES.items()
    }

    random.seed(42)
    TARGET_COUNT = 32

    manifest_rows: list[dict[str, str]] = []
    for class_name, source_dir in class_sources.items():
        if not source_dir.exists():
            raise FileNotFoundError(f"Missing source directory: {source_dir}")

        target_dir = args.data_dir / class_name
        target_dir.mkdir(parents=True, exist_ok=True)

        if args.clear:
            for existing in iter_audio_files(target_dir):
                existing.unlink()

        all_audio_paths = []
        for audio_path in iter_audio_files(source_dir):
            # exclude test_data directory from training set
            if "test_data" in audio_path.parts:
                continue
            all_audio_paths.append(audio_path)

        final_audio_paths = []

        if class_name == "not_rain":
            groups = defaultdict(list)
            for audio_path in all_audio_paths:
                relative_parts = audio_path.relative_to(source_dir).parts
                # use top-level subfolder as group key (e.g. ESC-50_101_Clock)
                group_name = relative_parts[0] if len(relative_parts) > 1 else "root"
                groups[group_name].append(audio_path)

            for group_name, paths in groups.items():
                if "ESC-50" in group_name and len(paths) > TARGET_COUNT:
                    final_audio_paths.extend(random.sample(paths, TARGET_COUNT))
                else:
                    final_audio_paths.extend(paths)
        else:
            final_audio_paths = all_audio_paths

        for audio_path in final_audio_paths:
            target_path = target_dir / safe_name(source_dir, audio_path)
            shutil.copy2(audio_path, target_path)
            manifest_rows.append(
                {
                    "class": class_name,
                    "source_path": str(audio_path),
                    "target_path": str(target_path),
                }
            )

    manifest_path = args.data_dir / "manifest.csv"
    with manifest_path.open("w", newline="", encoding="utf-8") as manifest_file:
        writer = csv.DictWriter(
            manifest_file,
            fieldnames=["class", "source_path", "target_path"],
        )
        writer.writeheader()
        writer.writerows(manifest_rows)


if __name__ == "__main__":
    main()

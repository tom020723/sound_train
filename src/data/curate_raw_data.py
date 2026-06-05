from __future__ import annotations

import argparse
import csv
import shutil
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

    manifest_rows: list[dict[str, str]] = []
    for class_name, source_dir in class_sources.items():
        if not source_dir.exists():
            raise FileNotFoundError(f"Missing source directory: {source_dir}")

        target_dir = args.data_dir / class_name
        target_dir.mkdir(parents=True, exist_ok=True)

        if args.clear:
            for existing in iter_audio_files(target_dir):
                existing.unlink()

        for audio_path in iter_audio_files(source_dir):
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

    print(f"Copied {len(manifest_rows)} files into {args.data_dir}")
    print(f"Saved manifest to {manifest_path}")


if __name__ == "__main__":
    main()

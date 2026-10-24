#!/usr/bin/env bash
set -euo pipefail

run=false
case "${1:-}" in
    '') ;;
    --run) run=true ;;
    -h|--help) echo "Usage: bash build.sh [--run]"; exit 0 ;;
    *) echo "Usage: bash build.sh [--run]" >&2; exit 1 ;;
esac
if (( $# > 1 )); then
    echo "Usage: bash build.sh [--run]" >&2
    exit 1
fi

root="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
build_dir="$root/build-linux"

for tool in cmake ctest; do
    if ! command -v "$tool" >/dev/null 2>&1; then
        echo "$tool is required. Install CMake, a C compiler, and Make (see README.md)." >&2
        exit 1
    fi
done

if [[ ! -f "$root/third_party/uthash/src/uthash.h" || ! -f "$root/third_party/Unity/src/unity.c" ]]; then
    git -C "$root" submodule update --init --recursive
fi

cmake -S "$root" -B "$build_dir" -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON
cmake --build "$build_dir"
ctest --test-dir "$build_dir" --output-on-failure
echo "Built: $build_dir/uthash_demo"
if "$run"; then
    "$build_dir/uthash_demo"
fi

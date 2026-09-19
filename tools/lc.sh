#!/usr/bin/env bash

set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
build_dir="${BUILD_DIR:-${repo_root}/build}"
build_type="${BUILD_TYPE:-Debug}"

usage() {
  echo "Usage:"
  echo "  $0 configure"
  echo "  $0 build <target>"
  echo "  $0 run <target>"
  echo "  $0 test [category]"
  echo "  $0 list"
}

configure() {
  cmake -S "${repo_root}" -B "${build_dir}" -DCMAKE_BUILD_TYPE="${build_type}"
}

require_target() {
  if [[ $# -lt 1 || -z "$1" ]]; then
    usage
    exit 2
  fi
}

case "${1:-}" in
  configure)
    configure
    ;;
  build)
    require_target "${2:-}"
    configure
    cmake --build "${build_dir}" --target "$2"
    ;;
  run)
    require_target "${2:-}"
    configure
    cmake --build "${build_dir}" --target "$2"
    "${build_dir}/bin/$2"
    ;;
  test)
    configure
    cmake --build "${build_dir}"
    if [[ -n "${2:-}" ]]; then
      ctest --test-dir "${build_dir}" -L "$2" --output-on-failure
    else
      ctest --test-dir "${build_dir}" --output-on-failure
    fi
    ;;
  list)
    configure
    cmake --build "${build_dir}" --target help
    ;;
  *)
    usage
    exit 2
    ;;
esac

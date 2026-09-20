#!/usr/bin/env bash

set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
build_dir="${BUILD_DIR:-${repo_root}/build}"
build_type="${BUILD_TYPE:-Debug}"

usage() {
  cat <<EOF
用法：
  $0 configure          配置 CMake
  $0 build [target]     配置并编译；省略 target 时编译全部已注册题目
  $0 run <target>       配置、编译并运行一道题
  $0 test [category]    配置、编译全部，再运行测试（可按标签筛选）
  $0 list               列出已注册题目测试
  $0 --help             显示帮助

示例：
  $0 build
  $0 run lc_704_binary_search
  $0 test binary_search

无需脚本也可构建，在仓库根目录执行：
  cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
  cmake --build build --parallel
  ctest --test-dir build --output-on-failure

环境变量：BUILD_DIR（默认：仓库/build），BUILD_TYPE（默认：Debug）。
EOF
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
  ""|-h|--help)
    usage
    ;;
  configure)
    configure
    ;;
  build)
    configure
    if [[ -n "${2:-}" ]]; then
      cmake --build "${build_dir}" --target "$2"
    else
      cmake --build "${build_dir}"
    fi
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
    ctest --test-dir "${build_dir}" -N
    ;;
  *)
    usage
    exit 2
    ;;
esac

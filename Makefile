.DEFAULT_GOAL := help
JOBS ?= 4

# Allow one problem name after an action: make build lc_704_binary_search.
ACTION := $(firstword $(MAKECMDGOALS))
ifneq ($(filter $(ACTION),build debug release run),)
PROBLEM := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
ifneq ($(word 2,$(PROBLEM)),)
$(error Specify only one problem, e.g. make build lc_704_binary_search)
endif
ifneq ($(filter $(PROBLEM),build debug release run test help),)
$(error Run one action at a time, e.g. make debug)
endif
ifneq ($(PROBLEM),)
.PHONY: $(PROBLEM)
$(PROBLEM):
	@:
endif
endif

ifeq ($(ACTION),run)
ifeq ($(PROBLEM),)
$(error Usage: make run <problem>, e.g. make run lc_704_binary_search)
endif
endif

.PHONY: help build debug release run test

help:
	@echo 'make build [problem]   Debug：保留调试信息和断言，只编译不运行'
	@echo 'make debug [problem]   同 make build'
	@echo 'make release [problem] Release：启用优化、禁用断言，只编译不运行'
	@echo 'make run <problem>     编译并运行单题（Debug）'
	@echo 'make test              编译并运行全部断言测试（Debug）'
	@echo '输出目录：Debug -> build/debug/bin/；Release -> build/release/bin/'
	@echo '日常刷题用 make run；验证全部题目用 make test。'
	@echo 'Release 会移除 assert 内的算法调用，正常退出不代表验证通过。'
	@echo '示例：make run lc_704_binary_search'

build: debug

debug:
	cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON
	+cmake --build build/debug --parallel $(JOBS) $(if $(PROBLEM),--target "$(PROBLEM)")

release:
	cmake -S . -B build/release -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF
	+cmake --build build/release --parallel $(JOBS) $(if $(PROBLEM),--target "$(PROBLEM)")

run: debug
	@./build/debug/bin/$(PROBLEM)

test: debug
	ctest --test-dir build/debug --output-on-failure

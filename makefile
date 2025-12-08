# World VTT
#
# Copyright (C) 2025, Asar Miniatures
# All rights reserved.
#
# This file is part of the [Project Name] project. It may be used, modified,
# and distributed under the terms specified by the copyright holder.

# ---------------------------------------
# World VTT / cpp_config — Master Makefile
# ---------------------------------------

PWD ?= $(shell pwd)

ifeq ($(V),1)
Q :=
else
Q ?= @
endif

SUDO ?= sudo

FORMATTER = clang-format-19
CPPCHECK  = cppcheck
CPPLINT   = cpplint

BUILD_DIR     = $(PWD)/build
BUILD_TYPE   ?= Debug

SRC_DIR       = $(PWD)/src
INCLUDE_DIR   = $(PWD)/include
TEST_DIR      = $(PWD)/tests

FIND_CMD = find $(SRC_DIR) $(INCLUDE_DIR) \( -iname "*.hpp" -o -iname "*.h" -o -iname "*.cpp" -o -iname "*.cppm" \)

.setup-clang:
	$(Q)echo "[SETUP] Installing LLVM Clang"
	$(Q)wget https://apt.llvm.org/llvm.sh
	$(Q)chmod +x llvm.sh
	$(Q)$(SUDO) ./llvm.sh
	$(Q)rm llvm.sh*

.setup-format:
	$(Q)$(SUDO) apt-get install -y clang-format-19

.setup-cpplint:
	$(Q)$(SUDO) apt-get install -y cpplint

.setup-cppcheck:
	$(Q)$(SUDO) apt-get install -y cppcheck

.setup-boost:
	$(Q)$(SUDO) apt-get install -y libboost-all-dev

.setup-cmake:
	$(Q)$(SUDO) apt-get update
	$(Q)$(SUDO) apt-get install -y cmake

setup: .setup-cppcheck .setup-cpplint .setup-format .setup-cmake .setup-boost
	$(Q)echo "[SETUP COMPLETE]"


build:
	@echo "[BUILD] Building project"
	$(Q)cmake -S . -B $(BUILD_DIR)/linux -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
	$(Q)cmake --build $(BUILD_DIR)/linux -j4

build-tests:
	@echo "[BUILD] Unit tests"
	$(Q)cmake -S . -B $(BUILD_DIR)/linux -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
	$(Q)cmake --build $(BUILD_DIR)/linux --target test_config -j4

test: build-tests
	@echo "[TEST] Running unit tests"
	$(Q)./build/linux/bin/test_config --gtest_output=xml:build/test-results.xml

clean:
	@echo "[CLEAN]"
	$(Q)rm -rf $(BUILD_DIR)
	$(Q)find . -name "*.o" -delete
	$(Q)find . -name "*.pcm" -delete

clang-check:
	@echo "[CLANG-FORMAT CHECK]"
	$(Q)$(FORMATTER) --version
	$(Q)$(FIND_CMD) | xargs $(FORMATTER) --dry-run --Werror

clang-reformat:
	@echo "[CLANG-FORMAT REFORMAT]"
	$(Q)$(FORMATTER) --version
	$(Q)$(FIND_CMD) | xargs $(FORMATTER) -i --Werror

cpplint:
	@echo "[CPPLINT]"
	$(Q)$(CPPLINT) --version
	$(Q)$(FIND_CMD) | xargs $(CPPLINT)

cppcheck:
	@echo "[CPPCHECK]"
	$(Q)$(CPPCHECK) --version
	$(Q)$(CPPCHECK) \
		--platform=native \
		--language=c++ \
		--std=c++23 \
		--enable=all \
		--inline-suppr \
		--force \
		-I $(INCLUDE_DIR) \
		$(SRC_DIR)


sync-github:
	git remote remove github 2>/dev/null || true
	git remote add github git@github.com:asarbs/cpp_config.git
	if [ -f .git/shallow ]; then git fetch --unshallow; fi
	git push --force github master --tags

all: clean clang-check cppcheck cpplint build test
	@echo "[ALL OK]"

.PHONY: build build-tests test clean clang-check clang-reformat cpplint cppcheck setup

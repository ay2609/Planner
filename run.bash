#!/bin/bash

#cmake -B ./build -S . -DCMAKE_TOOLCHAIN_FILE=./vcpkg_installed/arm64-osx/share/imgui/imgui-config.cmake
#cmake -B ./build -S . -DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake
cmake .
cmake --build .
./Planner
rm Planner

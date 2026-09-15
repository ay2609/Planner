# Planner

A desktop assignment/deadline tracker built with C++ and Dear ImGui.

## What it is

A native desktop app (GLFW + OpenGL3 + Dear ImGui) for tracking assignments. An `assignment`
class holds a title and due date, formatted with `fmt`. It includes a custom `imguidatechooser`
widget (a date picker), with RapidJSON available for save/load of assignment data.

## Stack

- C++, Dear ImGui, GLFW, OpenGL
- RapidJSON, fmt
- CMake + vcpkg

## Status

Work in progress — the UI shell (windowing, ImGui setup, date picker) is in place; persistence to
disk is still being wired up.

## Building

```bash
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=<path-to-vcpkg>/scripts/buildsystems/vcpkg.cmake
cmake --build build
```

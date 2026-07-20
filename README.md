# OpenGL Template

[![CMake](https://github.com/d3m1d0s/OpenGLTemplate/actions/workflows/cmake-multi-platform.yml/badge.svg?branch=main)](https://github.com/d3m1d0s/OpenGLTemplate/actions/workflows/cmake-multi-platform.yml)
![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)
![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)

## Table of Contents

- [Overview](#overview)
- [Build and Run](#build-and-run)
- [Features](#features)
- [Dependencies](#dependencies)
- [Using the Template](#using-the-template)
- [Example Output](#example-output)
- [Project Structure](#project-structure)
- [Troubleshooting](#troubleshooting)
- [License](#license)
- [Attribution](#attribution)

## Overview

This is a starter template for OpenGL projects in C++20. It opens a GLFW
window with an OpenGL 3.3 core profile context through GLEW and clears the
screen, leaving everything else to you. Dependencies are taken from the
system when installed and downloaded otherwise, so the project builds out
of the box on Windows, Linux and macOS.

## Build and Run

To create your own project from the template, use the "Use this template"
button on GitHub. To try it directly, ensure CMake 3.21 or newer and a
C++20 compiler are installed and run:

```
git clone https://github.com/d3m1d0s/OpenGLTemplate.git
cd OpenGLTemplate
cmake --preset default
cmake --build --preset default
./build/OpenGLTemplate
```

On Windows the executable is `build\OpenGLTemplate.exe`. The classic
`cmake -B build -S .` works as well if presets are not wanted. The project
can also be opened directly in CLion, which is free for non-commercial use
and picks the presets up by itself.

## Features

- C++20 and CMake 3.21 or newer.
- Windows, Linux and macOS, each covered by the CI matrix.
- Dependencies are searched with `find_package` first. Anything missing is
  downloaded as a release archive pinned by SHA256 and built statically as
  part of the project.
- GLFW 3.4 for the window and input, GLEW 2.2.0 as the OpenGL loader,
  GLM 1.0.1 for math, tinyobjloader for OBJ models and a vendored
  stb_image for textures.
- Optional Assimp and OpenCV, both off by default.
- CMake presets for the self-contained and the vcpkg workflows, plus a
  vcpkg manifest.
- `SHADERS_DIR` and `ASSETS_DIR` compile definitions, so asset loading
  does not depend on the working directory.
- Warning flags per compiler and automatic copying of runtime DLLs next
  to the executable on Windows.

## Dependencies

Nothing has to be preinstalled: whatever is missing gets downloaded at
configure time. System packages only speed configure up and let the
project link against shared system libraries.

### Windows

CMake plus Visual Studio or MinGW is enough for the self-contained build.
To use vcpkg instead, set the `VCPKG_ROOT` environment variable and build
with the vcpkg preset; the manifest installs glfw3, glew, glm and
tinyobjloader:

```
cmake --preset vcpkg
cmake --build --preset vcpkg
```

When configuring vcpkg manually (for example in the CLion CMake options),
pass the toolchain file and a triplet matching the compiler:

```
-DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake
-DVCPKG_TARGET_TRIPLET=x64-windows
```

### Linux

System packages on Debian and Ubuntu:

```
sudo apt install cmake g++ libglew-dev libglfw3-dev libglm-dev libtinyobjloader-dev
```

Without them GLFW and GLEW are built from source, which needs the X11 and
Wayland development packages:

```
sudo apt install cmake g++ xorg-dev libglu1-mesa-dev libwayland-dev wayland-protocols libxkbcommon-dev
```

### macOS

```
brew install cmake glfw glew glm
```

Homebrew has no tinyobjloader formula; the fallback covers it. OpenGL on
macOS is capped at version 4.1.

## Using the Template

### Shaders and assets

`shaders/` and `assets/` are empty and reserved for your files. The build
defines `SHADERS_DIR` and `ASSETS_DIR` with absolute paths to them, so
files load regardless of where the executable is started from:

```cpp
std::string vert = std::string(SHADERS_DIR) + "/basic.vert";
```

### Images and models

stb_image and tinyobjloader are already compiled into the project. Include
`<stb_image.h>` or `<tiny_obj_loader.h>` and use them. Never define
`STB_IMAGE_IMPLEMENTATION` or `TINYOBJLOADER_IMPLEMENTATION` yourself.
Both are already compiled, and a second definition breaks linking with
duplicate symbols.

### Optional libraries

Assimp and OpenCV are wired in but disabled. Enable them at configure
time:

```
cmake --preset default -DENABLE_ASSIMP=ON
```

With the vcpkg preset, also activate the matching manifest feature:

```
cmake --preset vcpkg -DENABLE_ASSIMP=ON -DVCPKG_MANIFEST_FEATURES=assimp
```

When Assimp is built from source, most of the work goes into its format
importers. The commented block in `CMakeLists.txt` shows how to keep only
the formats you need. Sources with optional code can check the
`USE_ASSIMP` and `USE_OPENCV` definitions.

### OpenGL version

The demo requests a 3.3 core profile context. To raise it, change the two
version window hints in `src/main.cpp`.

## Example Output

A white 800x600 window opens and stays until closed with ESC or the window
button. The console prints the context info:

```
OpenGL:   3.3.0 - Build 31.0.101.2140
Renderer: Intel(R) HD Graphics 620
GLEW:     2.2.0
```

## Project Structure

```
assets/            textures and models
include/           project headers
shaders/           GLSL files
src/               the demo application
third_party/stb/   vendored stb_image
CMakeLists.txt
CMakePresets.json
vcpkg.json
```

## Troubleshooting

- Linker errors like `__security_cookie` or `sscanf_s` on Windows mean the
  compiler and the vcpkg triplet do not match. MSVC pairs with
  `x64-windows` or `x64-windows-static`, MinGW with `x64-mingw-dynamic` or
  `x64-mingw-static`.
- CMake does not see vcpkg packages: the toolchain file was not passed.
  Use the vcpkg preset with `VCPKG_ROOT` set, or pass
  `-DCMAKE_TOOLCHAIN_FILE` manually.
- Configure fails on Linux while building GLFW: install the X11 and
  Wayland development packages listed above.
- `glewInit failed` in a Wayland session: distro GLEW is built for GLX, so
  the demo requests X11 (XWayland) at startup. If you remove that hint for
  native Wayland, use an EGL-built GLEW.
- The first configure downloads the missing dependencies into
  `build/_deps`; later configures reuse them.

## License

This project is authored by Demid Ostiakov and licensed under the
[MIT License](LICENSE).

## Attribution

This project was originally generated from
[d3m1d0s/OpenGLTemplate](https://github.com/d3m1d0s/OpenGLTemplate) (MIT).
If you use it as a template, please keep this line or move it to your
`NOTICE` file.

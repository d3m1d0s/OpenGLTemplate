# OpenGL Template

[![CMake](https://github.com/d3m1d0s/OpenGLTemplate/actions/workflows/cmake-multi-platform.yml/badge.svg?branch=main)](https://github.com/d3m1d0s/OpenGLTemplate/actions/workflows/cmake-multi-platform.yml)
![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)
![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)

## Table of Contents

- [Overview](#overview)
- [Build and Run](#build-and-run)
- [Features](#features)
- [Dependencies](#dependencies)
  - [Windows](#windows)
  - [Linux](#linux)
  - [macOS](#macos)
- [Using the Template](#using-the-template)
  - [Shaders and assets](#shaders-and-assets)
  - [Images and models](#images-and-models)
  - [Optional libraries](#optional-libraries)
  - [OpenGL version](#opengl-version)
- [Example Output](#example-output)
- [Project Structure](#project-structure)
- [Troubleshooting](#troubleshooting)
- [License](#license)
- [Acknowledgments](#acknowledgments)

## Overview

This is a starter template for OpenGL projects in C++20. It opens a GLFW
window with an OpenGL 3.3 core profile context through GLEW and clears the
screen, leaving everything else to you. Dependencies are taken from the
system when installed and downloaded otherwise, so the project builds out
of the box on Windows, Linux and macOS.

## Build and Run

To start your own project, click "Use this template" on GitHub and clone
the repository it creates:

```
git clone https://github.com/<user>/<your-new-repo>.git
```

To try the template as is, clone this repository instead:

```
git clone https://github.com/d3m1d0s/OpenGLTemplate.git
```

Either way, ensure CMake 3.21 or newer and a C++20 compiler are installed
and build inside the cloned folder:

```
cmake --preset default
cmake --build --preset default
./build/OpenGLTemplate
```

On Windows the executable is `build\OpenGLTemplate.exe`, and the classic
`cmake -B build -S .` works as well if presets are not wanted.

The project can also be opened directly in CLion, which is free for
non-commercial use and picks the presets up by itself.

## Features

- C++20 and CMake 3.21 or newer.
- Windows, Linux and macOS, each covered by the CI matrix.
- Dependencies are searched with `find_package` first. Anything missing is
  downloaded as a release archive pinned by SHA256 and built statically as
  part of the project.
- GLFW, GLEW, GLM, tinyobjloader and stb_image wired in and ready to use.
- Optional Assimp and OpenCV, both off by default.
- CMake presets for the self-contained and the vcpkg workflows, plus a
  vcpkg manifest.
- `SHADERS_DIR` and `ASSETS_DIR` compile definitions, so asset loading
  does not depend on the working directory.
- Warning flags per compiler and automatic copying of runtime DLLs next
  to the executable on Windows.

## Dependencies

Beyond CMake and a C++20 compiler, nothing has to be preinstalled: every
library below is searched on the system first and downloaded at configure
time when missing (the Linux from-source fallback additionally needs the
development packages listed further down). Installing libraries system-wide
only speeds the build up and lets the project link GLFW, GLEW and
tinyobjloader as shared system libraries; GLM is header-only either way.
Each library has one job:

- [GLFW](https://www.glfw.org/) 3.4 - opens the window, creates the OpenGL
  context and handles keyboard and mouse input
- [GLEW](https://glew.sourceforge.net/) 2.2.0 - loads the OpenGL function
  pointers at runtime, which portable code cannot rely on the system to export
- [GLM](https://github.com/g-truc/glm) 1.0.1 - vectors, matrices and the
  math behind transforms and cameras
- [tinyobjloader](https://github.com/tinyobjloader/tinyobjloader) - reads
  OBJ models with their MTL materials
- [Assimp](https://assimp.org/) - optional alternative to tinyobjloader
  that reads many more model formats, off by default
- [stb_image](https://github.com/nothings/stb) - reads PNG, JPEG and other
  image formats for textures, vendored in `third_party/stb`
- [OpenCV](https://opencv.org/) - optional alternative to stb_image that
  also processes images and video, off by default

### Windows

CMake plus Visual Studio or MinGW is enough for the self-contained build.
To use vcpkg instead, set the `VCPKG_ROOT` environment variable and build
with the vcpkg preset; the manifest installs glfw3, glew, glm and
tinyobjloader:

```
cmake --preset vcpkg
cmake --build --preset vcpkg
```

The optional libraries install through the same manifest as features,
for example Assimp:

```
cmake --preset vcpkg -DENABLE_ASSIMP=ON -DVCPKG_MANIFEST_FEATURES=assimp
```

When configuring vcpkg manually (for example in the CLion CMake options),
pass the toolchain file and a triplet matching the compiler:

```
-DCMAKE_TOOLCHAIN_FILE=<path-to-vcpkg>/scripts/buildsystems/vcpkg.cmake
-DVCPKG_TARGET_TRIPLET=x64-windows
```

Installing OpenCV through vcpkg builds it and its dependency chain from
source, which takes a long time. The faster route on Windows is the
official prebuilt release: download `opencv-<version>-windows.exe` from
the [OpenCV releases](https://github.com/opencv/opencv/releases) page
and run it; the file is a self-extracting archive that asks for a
destination folder. Then point the build at the unpacked tree:

```
cmake --preset default -DENABLE_OPENCV=ON -DOpenCV_DIR=<path>/opencv/build
```

### Linux

System packages on Debian and Ubuntu:

```
sudo apt install cmake g++ libglew-dev libglfw3-dev libglm-dev libtinyobjloader-dev
```

Without them GLFW and GLEW are built from source, which needs the OpenGL,
X11 and Wayland development packages:

```
sudo apt install cmake g++ xorg-dev libglu1-mesa-dev libwayland-dev wayland-protocols libxkbcommon-dev
```

For the optional Assimp and OpenCV:

```
sudo apt install libassimp-dev libopencv-dev
```

### macOS

```
brew install cmake glfw glew glm
```

Homebrew has no tinyobjloader formula; the fallback covers it.

For the optional Assimp and OpenCV:

```
brew install assimp opencv
```

OpenGL on macOS is capped at version 4.1.

## Using the Template

### Shaders and assets

`shaders/` and `assets/` hold only `.gitkeep` placeholders and are reserved
for your files. The build
defines `SHADERS_DIR` and `ASSETS_DIR` with absolute paths to them, so
files load regardless of where the executable is started from:

```cpp
std::string vert = std::string(SHADERS_DIR) + "/basic.vert";
```

### Images and models

stb_image is compiled into the project, and tinyobjloader is linked in as
well, either as a prebuilt package when one is found or built from the
fetched sources otherwise. Include `<stb_image.h>` or `<tiny_obj_loader.h>`
and use them. Never define `STB_IMAGE_IMPLEMENTATION` or
`TINYOBJLOADER_IMPLEMENTATION` yourself. Both are already compiled, and
defining the macros again in your own files risks duplicate-symbol linker
errors.

### Optional libraries

Assimp and OpenCV are wired in but disabled. Enable either one at
configure time, and gate the code that uses it with the matching
`USE_ASSIMP` or `USE_OPENCV` definition:

```
cmake --preset default -DENABLE_ASSIMP=ON -DENABLE_OPENCV=ON
```

Assimp behaves like the core dependencies: when no installed copy is
found, the pinned sources are downloaded and built as part of the
project. That build spends most of its time on the format importers, and
the commented block in `CMakeLists.txt` shows how to keep only the
formats you need.

OpenCV is the one dependency without a download fallback, so it has to
be installed first; the commands for every system are in
[Dependencies](#dependencies). When the installed OpenCV is a shared
library, the template copies its DLL next to the executable on Windows
automatically.

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

With the optional libraries enabled, the demo also prints their
versions.

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

Distributed under the MIT License. See [LICENSE](LICENSE) for more
information. Attribution is appreciated.

## Acknowledgments

The vendored `stb_image.h` comes from Sean Barrett's
[stb](https://github.com/nothings/stb) libraries, dual-licensed MIT and
public domain.

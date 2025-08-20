# OpenGLTemplate

[![CMake](https://github.com/d3m1d0s/OpenGLTemplate/actions/workflows/cmake-multi-platform.yml/badge.svg?branch=main)](https://github.com/d3m1d0s/OpenGLTemplate/actions/workflows/cmake-multi-platform.yml)
![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)
![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)

A modern **C++20 OpenGL starter template** with [GLFW](https://www.glfw.org/) and [GLAD](https://glad.dav1d.de/).
Works **out-of-the-box**: uses preinstalled packages (vcpkg/Conan/system) or auto-fetches sources via CMake.

---

## 📚 Table of Contents
- [⚡ Quick Start](#-quick-start)
- [💻 Recommended IDE: CLion](#-recommended-ide-clion)
- [✨ Features](#-features)
- [📦 Dependencies](#-dependencies)
- [🖼️ Example Output](#-example-output)
- [📂 Project Structure](#-project-structure)
- [🆘 Troubleshooting](#-troubleshooting)
- [📜 License](#-license)
- [🙌 Attribution](#-attribution)

---

## ⚡ Quick Start
Tip: It’s recommended to preinstall [Dependencies](#-dependencies).
If something goes wrong - check [Troubleshooting](#-troubleshooting).

```bash
git clone https://github.com/<youruser>/<yourrepo>.git
cd OpenGLTemplate
cmake -B build -S .
cmake --build build
./build/OpenGLTemplate   # Windows: .\build\OpenGLTemplate.exe
```

---

## 💻 Recommended IDE: CLion

[JetBrains CLion](https://www.jetbrains.com/clion/) is a great choice for CMake/C++ projects.
Just open this folder in CLion and it will automatically detect the `CMakeLists.txt` and configure everything.

Tip: if you use **vcpkg** with CLion, set a toolchain file in
`Settings → Build, Execution, Deployment → CMake → CMake options` and pick a proper triplet (e.g. `x64-windows-static`):

```
-DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake 
-DVCPKG_TARGET_TRIPLET=x64-windows-static
```

---

## ✨ Features

* ⚙️ Minimal CMake (>= 3.20), C++20.
* 🌍 Cross-platform: Windows, Linux, macOS.
* 🔀 Dual dependency strategy:

  * Uses `find_package` if `glfw3` / `glad` are installed.
  * Otherwise `FetchContent` pulls GLFW 3.4 & GLAD 0.1.36.
* 🔗 Clean linking: `OpenGL::GL`, `glfw`, `glad`.

---

## 📦 Dependencies

### 🪟 Windows (via vcpkg)

```powershell
vcpkg install glfw3 glad
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows-static
```

> Static triplets easiest (no DLLs). Dynamic require shipping `glfw3.dll`.

### 🐧 Linux (system packages)

```bash
sudo apt install cmake g++ pkg-config libglfw3-dev libgl1-mesa-dev
sudo apt install libglad-dev    # If glad is not available via your package manager, no worries - this template will fetch it automatically.
cmake -B build -S .
cmake --build build
```

### 🍏 macOS (Homebrew)

```bash
brew install cmake glfw
brew install glad        # If glad is not available via your package manager, no worries - this template will fetch it automatically.
cmake -B build -S .
cmake --build build
```

> macOS OpenGL is capped at **4.1**.

### 📦 Self-contained (no pkg manager)

Do nothing - CMake auto-fetches **GLFW 3.4** and **GLAD 0.1.36**.

---

## 🖼️ Example Output

Opens a **800×600 window**, clears to white, prints OpenGL version, e.g.:

```
OpenGL: 3.3.0 - Build 31.0.101.2135
```

---

## 📂 Project Structure

```
OpenGLTemplate/
├─ src/              # C++ sources (main.cpp included)
├─ include/          # Headers
├─ shaders/          # GLSL files
├─ assets/           # Textures, models
├─ CMakeLists.txt
└─ README.md
```

> To bump OpenGL version, edit in CMake:
> `set(GLAD_API "gl=4.6")`.

## 🆘 Troubleshooting

- **Windows: mixing toolchains/triplets**  
  Errors like `__security_cookie`, `sscanf_s`, or `fprintf` usually mean you’re compiling with **MinGW** but linked **MSVC** vcpkg triplets (or vice versa).  
  Align them:
  - MSVC ↔ `x64-windows` / `x64-windows-static`
  - MinGW-w64 ↔ `x64-mingw-dynamic` / `x64-mingw-static`

- **CMake can’t find `glfw3Config.cmake`**  
  If using vcpkg, make sure you pass the toolchain file and a triplet:
  ```bash
  -DCMAKE_TOOLCHAIN_FILE=<path-to-vcpkg>/scripts/buildsystems/vcpkg.cmake
  -DVCPKG_TARGET_TRIPLET=x64-windows-static   # or your platform

- **DLLs missing on Windows (dynamic triplets)**  
   Copy runtime DLLs (e.g., `glfw3.dll`) next to your executable or use a static triplet.

- **Linux: missing dev packages**  
   Some distros don’t ship certain OpenGL helpers (e.g., `libglad-dev`).
  On Ubuntu/Debian, enable `universe` or just skip it — this template will **auto-fetch GLAD** via `FetchContent`.

---
## 📜 License

This project is licensed under the [MIT License](LICENSE).

---
## 🙌 Attribution

This project was originally generated from [d3m1d0s/OpenGLTemplate](https://github.com/d3m1d0s/OpenGLTemplate) — MIT.
If you use this as a template, please keep this line or move it to your `NOTICE` file.

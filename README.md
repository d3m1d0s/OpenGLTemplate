# OpenGLTemplate

[![CI](https://github.com/OWNER/REPO/actions/workflows/cmake-multi-platform.yml/badge.svg?branch=main)](https://github.com/OWNER/REPO/actions/workflows/cmake-multi-platform.yml)
[![CMake](https://github.com/d3m1d0s/OpenGLTemplate/actions/workflows/cmake-multi-platform.yml/badge.svg?branch=main)](https://github.com/d3m1d0s/OpenGLTemplate/actions/workflows/cmake-multi-platform.yml)
![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)
![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)

> **Using this as a template?**  
> After clicking **Use this template** (or `gh repo create --template d3m1d0s/OpenGLTemplate`) all occurrences of `OWNER/REPO` will be replaced with `<youruser>/<yourrepo>` automatically in:
> - CI badge(s)
> - Quick Start clone URL
> - Any other links to Actions

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
- [📜 License](#-license)
- [🙌 Attribution](#-attribution)

---

## ⚡ Quick Start
Tip: It’s recommended to preinstall [Dependencies](#-dependencies).


```bash
git clone https://github.com/OWNER/REPO.git
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
sudo apt install cmake g++ pkg-config libglfw3-dev libgl1-mesa-dev libglad-dev
cmake -B build -S .
cmake --build build
```

### 🍏 macOS (Homebrew)

```bash
brew install cmake glfw glad
cmake -B build -S .
cmake --build build
```

> macOS OpenGL is capped at **4.1**.

### 📦 Self-contained (no pkg manager)

Do nothing — CMake auto-fetches **GLFW 3.4** and **GLAD 0.1.36**.

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

---
## 📜 License

This project is licensed under the [MIT License](LICENSE).

---
## 🙌 Attribution

This project was originally generated from [d3m1d0s/OpenGLTemplate](https://github.com/d3m1d0s/OpenGLTemplate) — MIT.
If you use this as a template, please keep this line or move it to your `NOTICE` file.

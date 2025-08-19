cmake_minimum_required(VERSION 3.20)
project(OpenGLTemplate LANGUAGES C CXX)

# Use C++20 features in this project (GLFW/GLAD themselves are C, but this project is C++).
set(CMAKE_CXX_STANDARD 20)

# Look for a system/OpenGL SDK and create the imported target OpenGL::GL.
find_package(OpenGL REQUIRED)

# Try to find preinstalled packages first (works with vcpkg, Conan, system installs).
# The "CONFIG" mode searches for package config files (glfw3Config.cmake, gladConfig.cmake).
find_package(glfw3 CONFIG QUIET)
find_package(glad  CONFIG QUIET)


# Fallback path: if a package is missing, fetch its sources and build it as part of this project.
# This keeps the project self-contained and avoids requiring users to install dependencies manually.

# --- GLFW fallback (FetchContent) ---
if (NOT glfw3_FOUND)
    include(FetchContent)
    # Pin to a known GLFW release; adjust GIT_TAG when you want to upgrade.
    FetchContent_Declare(glfw
            GIT_REPOSITORY https://github.com/glfw/glfw.git
            GIT_TAG 3.4
    )
    # Build only the library; skip extras to speed up configure/build times.
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(glfw)
endif()

# --- GLAD fallback (FetchContent) ---
if (NOT glad_FOUND)
    include(FetchContent)
    # Pin to a GLAD release; GLAD's CMake can generate the loader at build time.
    FetchContent_Declare(glad
            GIT_REPOSITORY https://github.com/Dav1dde/glad.git
            GIT_TAG v0.1.36
    )
    # Configure the generated loader:
    # - Use the "core" profile to access modern OpenGL without deprecated functions.
    # - Request OpenGL 3.3; raise this (e.g., "gl=4.6") if your hardware/drivers allow.
    set(GLAD_PROFILE "core" CACHE STRING "" FORCE)
    set(GLAD_API     "gl=3.3" CACHE STRING "" FORCE)
    FetchContent_MakeAvailable(glad)
endif()

# Normalize target names so the rest of the CMakeLists can link the same variables
# regardless of whether the libs came from a package manager or from sources.
if (TARGET glfw)        # when built from sources
    set(GLFW_TARGET glfw)
elseif (TARGET glfw::glfw)  # when found via package (e.g., vcpkg)
    set(GLFW_TARGET glfw::glfw)
else()
    message(FATAL_ERROR "GLFW target not found")
endif()

if (TARGET glad)        # when built from sources
    set(GLAD_TARGET glad)
elseif (TARGET glad::glad)  # when found via package (e.g., vcpkg)
    set(GLAD_TARGET glad::glad)
else()
    message(FATAL_ERROR "GLAD target not found")
endif()

# Your application target. Add more source files here as your project grows.
add_executable(OpenGLTemplate src/main.cpp)

# Link against:
# - OpenGL::GL: the system OpenGL implementation
# - ${GLFW_TARGET}: window/context/input handling
# - ${GLAD_TARGET}: function loader for the requested OpenGL version/profile
target_link_libraries(OpenGLTemplate PRIVATE
        OpenGL::GL
        ${GLFW_TARGET}
        ${GLAD_TARGET}
)



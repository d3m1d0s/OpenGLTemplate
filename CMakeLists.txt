cmake_minimum_required(VERSION 3.20)
project(OpenGLTemplate LANGUAGES C CXX)

# Use C++20 features in this project (GLFW/GLAD themselves are C, but this project is C++).
set(CMAKE_CXX_STANDARD 20)

# Look for a system/OpenGL SDK and create the imported target OpenGL::GL.
find_package(OpenGL REQUIRED)

# -----------------------------
# 0) Options (On/Off add. libs)
# -----------------------------
option(ENABLE_ASSIMP      "Enable Assimp (model loading)"          ON)
option(ENABLE_OPENCV      "Enable OpenCV (images/video)"           ON)
option(ENABLE_ANTTWEAKBAR "Enable AntTweakBar (debug UI sliders)"  ON)

# ---- Try preinstalled packages (vcpkg/Conan/system) ----
find_package(glfw3 CONFIG QUIET)
find_package(glad  CONFIG QUIET)
find_package(glm   CONFIG QUIET)

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
    #  for legacy-examples - compatability
    set(GLAD_PROFILE "compatibility" CACHE STRING "" FORCE)
    set(GLAD_API     "gl=3.3"         CACHE STRING "" FORCE)
    FetchContent_MakeAvailable(glad)
endif()

if (NOT glm_FOUND)
    include(FetchContent)
    FetchContent_Declare(glm
            GIT_REPOSITORY https://github.com/g-truc/glm.git
            GIT_TAG 1.0.1
    )
    FetchContent_MakeAvailable(glm)
endif()

# -------- Assimp (fallback) --------
if (ENABLE_ASSIMP)
    find_package(assimp CONFIG QUIET)
    if (NOT assimp_FOUND)
        include(FetchContent)
        FetchContent_Declare(assimp
                GIT_REPOSITORY https://github.com/assimp/assimp.git
                GIT_TAG v5.4.3
        )
        # Disable treating warnings as errors (important for MinGW/GCC builds).
        set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
        set(ENABLE_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)

        # Trim down the build to speed it up
        set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
        set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
        set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)

        # Optional: restrict importers to specific formats (e.g. FBX/OBJ only).
        # set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
        # set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "" FORCE)
        # set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)

        FetchContent_MakeAvailable(assimp)

        # After the 'assimp' target is available, silence problematic warnings
        # under GCC/Clang (MSVC-specific pragmas etc.).
        if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang" AND TARGET assimp)
            target_compile_options(assimp PRIVATE -Wno-unknown-pragmas -Wno-error)
        endif()
    endif()
endif()


# -------- OpenCV (optional) --------
if (ENABLE_OPENCV)
    # In vcpkg target usually is OpenCV::opencv or set of modules (opencv_core, ...)
    find_package(OpenCV CONFIG QUIET)
    if (OpenCV_FOUND)
        if (TARGET OpenCV::opencv)
            set(OPENCV_TARGETS OpenCV::opencv)
        elseif (DEFINED OpenCV_LIBS)
            set(OPENCV_TARGETS ${OpenCV_LIBS})
        elseif (DEFINED OpenCV_LIBRARIES)
            set(OPENCV_TARGETS ${OpenCV_LIBRARIES})
        endif()
    endif()
endif()

# -------- AntTweakBar (optional) --------
if (ENABLE_ANTTWEAKBAR)
    find_package(AntTweakBar CONFIG QUIET) # vcpkg: anttweakbar
    if (TARGET AntTweakBar::AntTweakBar)
        set(ATB_TARGET AntTweakBar::AntTweakBar)
    elseif (TARGET AntTweakBar)
        set(ATB_TARGET AntTweakBar)
    endif()
endif()

# ---- Normalize target names ----
# GLFW
if (TARGET glfw)
    set(GLFW_TARGET glfw)
elseif (TARGET glfw::glfw)
    set(GLFW_TARGET glfw::glfw)
else()
    message(FATAL_ERROR "GLFW target not found")
endif()

# GLAD
if (TARGET glad)
    set(GLAD_TARGET glad)
elseif (TARGET glad::glad)
    set(GLAD_TARGET glad::glad)
else()
    message(FATAL_ERROR "GLAD target not found")
endif()

# GLM (header-only)
if (TARGET glm::glm)
    set(GLM_TARGET glm::glm)
elseif (TARGET glm)
    set(GLM_TARGET glm)
else()
    message(FATAL_ERROR "GLM target not found")
endif()

# Assimp (optional)
if (ENABLE_ASSIMP)
    if (TARGET assimp::assimp)
        set(ASSIMP_TARGET assimp::assimp)
    elseif (TARGET assimp)
        set(ASSIMP_TARGET assimp)
    endif()
endif()

# ---- App target ----
add_executable(OpenGLTemplate src/main.cpp)

target_link_libraries(OpenGLTemplate PRIVATE
        OpenGL::GL
        ${GLFW_TARGET}
        ${GLAD_TARGET}
        ${GLM_TARGET}
)

# Link optional libs if found
if (ASSIMP_TARGET)
    target_link_libraries(OpenGLTemplate PRIVATE ${ASSIMP_TARGET})
    target_compile_definitions(OpenGLTemplate PRIVATE USE_ASSIMP=1)
endif()

if (OPENCV_TARGETS)
    target_link_libraries(OpenGLTemplate PRIVATE ${OPENCV_TARGETS})
    # If not imported target, then adding include-directories
    if (DEFINED OpenCV_INCLUDE_DIRS AND NOT TARGET OpenCV::opencv)
        target_include_directories(OpenGLTemplate PRIVATE ${OpenCV_INCLUDE_DIRS})
    endif()
    target_compile_definitions(OpenGLTemplate PRIVATE USE_OPENCV=1)
endif()

if (ATB_TARGET)
    target_link_libraries(OpenGLTemplate PRIVATE ${ATB_TARGET})
    target_compile_definitions(OpenGLTemplate PRIVATE USE_ANTTWEAKBAR=1)
endif()

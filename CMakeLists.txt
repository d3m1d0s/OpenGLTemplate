# 3.21 is needed for the $<TARGET_RUNTIME_DLLS> expression used below.
cmake_minimum_required(VERSION 3.21)
project(OpenGLTemplate VERSION 1.0.0 LANGUAGES C CXX)

# The project itself is C++20. The C language is needed for stb_image_impl.c
# and the fetched C libraries.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Look for a system/OpenGL SDK and create the imported target OpenGL::GL.
find_package(OpenGL REQUIRED)

# ---- Optional libraries (toggle on/off) ----
option(ENABLE_ASSIMP "Enable Assimp (model loading)"  OFF)
option(ENABLE_OPENCV "Enable OpenCV (images/video)"   OFF)

# ---- Try preinstalled packages (vcpkg/Conan/system) ----
find_package(glfw3 CONFIG QUIET)
find_package(GLEW QUIET) # Plain find_package also finds GLEW installs that ship no CMake config file,
# like apt's libglew-dev.
find_package(glm CONFIG QUIET)
find_package(tinyobjloader CONFIG QUIET)

# Fallback path: if a package is missing, fetch its sources and build it as part of this project.
# This keeps the project self-contained and avoids requiring users to install dependencies manually.
set(BUILD_SHARED_LIBS OFF) # fallback libraries are built as static libs

# --- GLFW fallback (FetchContent) ---
if (NOT TARGET glfw)
    include(FetchContent)
    message(STATUS "glfw3 not found - fetching GLFW 3.4")
    FetchContent_Declare(glfw
            URL      https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip
            URL_HASH SHA256=b5ec004b2712fd08e8861dc271428f048775200a2df719ccf575143ba749a3e9
    )
    # Both are ON by default and would otherwise end up in this project's build.
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL    OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(glfw)
endif()

# --- GLEW fallback (FetchContent) ---
if (NOT TARGET GLEW::GLEW)
    include(FetchContent)
    message(STATUS "GLEW not found - fetching GLEW 2.2.0")
    # GLEW's git repo holds only the code generator, not the glew.c/glew.h it
    # produces, so a git clone would not build. The release archive ships the
    # generated files and keeps its CMake project in the build/cmake subdirectory.
    FetchContent_Declare(glew
            URL      https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0.tgz
            URL_HASH SHA256=d4fc82893cfb00109578d0a1a2337fb8ca335b3ceccf97b97e5cc7f08e4353e1
            SOURCE_SUBDIR build/cmake
    )
    # BUILD_UTILS OFF skips the glewinfo/visualinfo tools. GLEW always builds both
    # the shared and the static library, so do not be surprised by a glew32.dll
    # in the build tree. Only the static glew_s is linked here.
    set(BUILD_UTILS OFF CACHE BOOL "" FORCE)
    # GLEW 2.2.0 declares cmake_minimum_required(2.8.12) and CMake 4.x rejects
    # projects declaring anything below 3.5. The minimum is overridden to 3.5
    # for GLEW's configure step and reset right after.
    set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
    FetchContent_MakeAvailable(glew)
    unset(CMAKE_POLICY_VERSION_MINIMUM)
    if (NOT TARGET GLEW::GLEW)
        # Statically linked GLEW normally requires consumers to define GLEW_STATIC,
        # otherwise linking fails on Windows. The glew_s target spreads that define
        # by itself, which is why it appears nowhere in this file.
        add_library(GLEW::GLEW ALIAS glew_s)
    endif()
    # The fetched GLEW is never installed, so its headers must be added explicitly.
    target_include_directories(glew_s PUBLIC $<BUILD_INTERFACE:${glew_SOURCE_DIR}/include>)
endif()

# --- GLM fallback (FetchContent) ---
if (NOT TARGET glm::glm AND NOT TARGET glm)
    include(FetchContent)
    message(STATUS "glm not found - fetching GLM 1.0.1")
    FetchContent_Declare(glm
            URL      https://github.com/g-truc/glm/archive/refs/tags/1.0.1.zip
            URL_HASH SHA256=09c5716296787e1f7fcb87b1cbdbf26814ec1288ed6259ccd30d5d9795809fa5
    )
    FetchContent_MakeAvailable(glm)
endif()

# --- tinyobjloader fallback (FetchContent) ---
if (NOT TARGET tinyobjloader::tinyobjloader)
    include(FetchContent)
    message(STATUS "tinyobjloader not found - fetching v2.0.0rc13")
    FetchContent_Declare(tinyobjloader
            URL      https://github.com/tinyobjloader/tinyobjloader/archive/refs/tags/v2.0.0rc13.tar.gz
            URL_HASH SHA256=0feb92b838f8ce4aa6eb0ccc32dff30cb64a891e0ec3bde837fca49c78d44334
    )
    FetchContent_MakeAvailable(tinyobjloader)
    # The tinyobjloader:: namespace only exists in installed packages,
    # so the alias is created here.
    add_library(tinyobjloader::tinyobjloader ALIAS tinyobjloader)
endif()

# --- stb_image (vendored) ---
# stb_image is vendored rather than searched for, because package managers cannot
# agree on how to ship stb. A committed header works the same everywhere.
add_library(stb_image STATIC third_party/stb/stb_image_impl.c)
target_include_directories(stb_image SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/stb)
add_library(stb::image ALIAS stb_image)

# -------- Assimp (fallback) --------
if (ENABLE_ASSIMP)
    find_package(assimp CONFIG QUIET)
    if (NOT assimp_FOUND)
        include(FetchContent)
        message(STATUS "assimp not found - fetching Assimp v5.4.3")
        FetchContent_Declare(assimp
                GIT_REPOSITORY https://github.com/assimp/assimp.git
                GIT_TAG v5.4.3
        )
        # Disable treating warnings as errors (important for MinGW/GCC builds).
        set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)

        # Trim down the build to speed it up
        set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
        set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)

        # Optional: restrict importers to specific formats (e.g. FBX/OBJ only).
        # set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
        # set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "" FORCE)
        # set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)

        FetchContent_MakeAvailable(assimp)
    endif()
endif()


# -------- OpenCV (optional) --------
if (ENABLE_OPENCV)
    # OpenCV has no namespaced OpenCV:: target. Its config file lists the module
    # targets (opencv_core, ...) in OpenCV_LIBS, so plain variables are used below.
    find_package(OpenCV CONFIG QUIET)
    if (OpenCV_FOUND)
        set(OPENCV_TARGETS ${OpenCV_LIBS})
    else()
        message(WARNING "ENABLE_OPENCV is ON but OpenCV was not found; building without it")
    endif()
endif()

# ---- Normalize target names ----
# GLFW
if (NOT TARGET glfw)
    message(FATAL_ERROR "GLFW target not found")
endif()

# GLEW
if (TARGET GLEW::GLEW)
    set(GLEW_TARGET GLEW::GLEW)
else()
    message(FATAL_ERROR "GLEW target not found")
endif()

# GLM
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

target_include_directories(OpenGLTemplate PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Absolute paths into the source tree, so asset loading does not depend
# on the working directory.
target_compile_definitions(OpenGLTemplate PRIVATE
        SHADERS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/shaders"
        ASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets"
)

target_compile_options(OpenGLTemplate PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall;-Wextra>
)

target_link_libraries(OpenGLTemplate PRIVATE
        OpenGL::GL
        glfw
        ${GLEW_TARGET}
        ${GLM_TARGET}
        tinyobjloader::tinyobjloader
        stb::image
)

# Link optional libs if found
if (ASSIMP_TARGET)
    target_link_libraries(OpenGLTemplate PRIVATE ${ASSIMP_TARGET})
    target_compile_definitions(OpenGLTemplate PRIVATE USE_ASSIMP=1)
endif()

if (OPENCV_TARGETS)
    target_link_libraries(OpenGLTemplate PRIVATE ${OPENCV_TARGETS})
    # Older OpenCV configs expose plain library names that carry no include paths,
    # so the include directories are added manually.
    if (DEFINED OpenCV_INCLUDE_DIRS)
        target_include_directories(OpenGLTemplate PRIVATE ${OpenCV_INCLUDE_DIRS})
    endif()
    target_compile_definitions(OpenGLTemplate PRIVATE USE_OPENCV=1)
endif()

# Copy runtime DLLs next to the executable, so it runs straight from the build
# tree. The $<IF> guard is there because copy_if_different fails when the DLL
# list is empty, as in all-static builds.
if (WIN32)
    add_custom_command(TARGET OpenGLTemplate POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E $<IF:$<BOOL:$<TARGET_RUNTIME_DLLS:OpenGLTemplate>>,copy_if_different,true> $<TARGET_RUNTIME_DLLS:OpenGLTemplate> $<TARGET_FILE_DIR:OpenGLTemplate>
            COMMAND_EXPAND_LISTS)
endif()

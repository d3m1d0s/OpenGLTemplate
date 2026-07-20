#define GLFW_INCLUDE_NONE
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#ifdef USE_ASSIMP
#include <assimp/version.h>
#endif
#ifdef USE_OPENCV
#include <opencv2/core/utility.hpp>
#endif

static void framebuffer_size_callback(GLFWwindow*, int w, int h){ glViewport(0,0,w,h); }

static void key_callback(GLFWwindow* win, int key, int, int action, int){
    if(key==GLFW_KEY_ESCAPE && action==GLFW_PRESS) glfwSetWindowShouldClose(win, GLFW_TRUE);
}

int main(){
#if defined(__linux__) && defined(GLFW_PLATFORM)
    // Distro GLEW is usually built for GLX and fails to initialize on a native
    // Wayland session, so X11 (XWayland on Wayland desktops) is requested instead.
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
#endif
    if(!glfwInit()){ std::cerr<<"GLFW init failed\n"; return -1; }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    // Required on macOS to create a core-profile context.
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GLFW_TRUE);
#endif

    GLFWwindow* win = glfwCreateWindow(800,600,"GLFW+GLEW",nullptr,nullptr);
    if(!win){ std::cerr<<"CreateWindow failed\n"; glfwTerminate(); return -1; }
    glfwMakeContextCurrent(win);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);
    glfwSetKeyCallback(win, key_callback);

    // glewInit must run after a GL context is current.
    if(GLenum err = glewInit(); err != GLEW_OK){
        std::cerr<<"glewInit failed: "<<glewGetErrorString(err)<<"\n";
        glfwTerminate();
        return -1;
    }
    std::cout<<"OpenGL:   "<<glGetString(GL_VERSION)<<"\n";
    std::cout<<"Renderer: "<<glGetString(GL_RENDERER)<<"\n";
    std::cout<<"GLEW:     "<<glewGetString(GLEW_VERSION)<<std::endl;
#ifdef USE_ASSIMP
    std::cout<<"Assimp:   "<<aiGetVersionMajor()<<"."<<aiGetVersionMinor()<<"."<<aiGetVersionPatch()<<std::endl;
#endif
#ifdef USE_OPENCV
    std::cout<<"OpenCV:   "<<cv::getVersionString()<<std::endl;
#endif

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    while(!glfwWindowShouldClose(win)){
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(win);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

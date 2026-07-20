#define GLFW_INCLUDE_NONE
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

static void framebuffer_size_callback(GLFWwindow*, int w, int h){ glViewport(0,0,w,h); }

static void key_callback(GLFWwindow* win, int key, int, int action, int){
    if(key==GLFW_KEY_ESCAPE && action==GLFW_PRESS) glfwSetWindowShouldClose(win, GLFW_TRUE);
}

int main(){
#if defined(__linux__) && defined(GLFW_PLATFORM)
    // Prefer X11 (XWayland on Wayland desktops): distro GLEW is usually built
    // for GLX and glewInit() can fail on a native Wayland/EGL context.
    // Remove this hint if you build an EGL-flavored GLEW for native Wayland.
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
#endif
    if(!glfwInit()){ std::cerr<<"GLFW init failed\n"; return -1; }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    // macOS refuses to create a 3.x core-profile context without this.
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GLFW_TRUE);
#endif

    GLFWwindow* win = glfwCreateWindow(800,600,"GLFW+GLEW",nullptr,nullptr);
    if(!win){ std::cerr<<"CreateWindow failed\n"; glfwTerminate(); return -1; }
    glfwMakeContextCurrent(win);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);
    glfwSetKeyCallback(win, key_callback);

    // glewInit must run after a GL context is current.
    glewExperimental = GL_TRUE;
    if(GLenum err = glewInit(); err != GLEW_OK){
        std::cerr<<"glewInit failed: "<<glewGetErrorString(err)<<"\n";
        glfwTerminate();
        return -1;
    }
    std::cout<<"OpenGL:   "<<glGetString(GL_VERSION)<<"\n";
    std::cout<<"Renderer: "<<glGetString(GL_RENDERER)<<"\n";
    std::cout<<"GLEW:     "<<glewGetString(GLEW_VERSION)<<std::endl;

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    while(!glfwWindowShouldClose(win)){
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(win);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

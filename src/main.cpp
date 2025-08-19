#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

static void framebuffer_size_callback(GLFWwindow*, int w, int h){ glViewport(0,0,w,h); }

int main(){
    if(!glfwInit()){ std::cerr<<"GLFW init failed\n"; return -1; }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* win = glfwCreateWindow(800,600,"GLFW+GLAD",nullptr,nullptr);
    if(!win){ std::cerr<<"CreateWindow failed\n"; glfwTerminate(); return -1; }
    glfwMakeContextCurrent(win);
    glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);

    if(!gladLoadGL()){
        std::cerr<<"gladLoadGL failed\n";
        return -1;
    }
    std::cout<<"OpenGL: "<<glGetString(GL_VERSION)<<"\n";

    while(!glfwWindowShouldClose(win)){
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(win);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

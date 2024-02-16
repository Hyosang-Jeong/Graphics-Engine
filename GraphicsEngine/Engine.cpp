#include"Engine.h"
#include<iostream>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
Engine::Engine()
{}

Engine::~Engine()
{
    glfwTerminate();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    shaderManager.UnLoad();
    sceneManager.UnLoad();
    modelManager.UnLoad();
}

void Engine::init(unsigned w, unsigned h)
{
    if (!glfwInit())
    {
        std::cout << "Unable to create OpenGL context" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_RED_BITS, 8); glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8); glfwWindowHint(GLFW_ALPHA_BITS, 8);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); // window dimensions are static

    ptr_window = glfwCreateWindow(w, h, "Graphics", NULL, NULL);

    if (!ptr_window) {
        std::cerr << "GLFW unable to create OpenGL context - abort program\n";
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(ptr_window);
    glfwSetInputMode(ptr_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glewInit();
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(ptr_window, true);
    const char* glsl_version = "#version 450";
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImGui::StyleColorsDark();


    camera = new Camera(ptr_window, { 0.0f, 2.f, 12.0f });
    windowSize.x = w;
    windowSize.y = h;
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
}

void Engine::Update()
{
    glfwPollEvents();
    if (glfwGetKey(ptr_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(ptr_window, GLFW_TRUE);
        return;
    }
    static double prev_time = glfwGetTime();
    double curr_time = glfwGetTime();
    double delta_time = curr_time - prev_time;
    prev_time = curr_time;

    sceneManager.GetScene()->Update(static_cast<float>(delta_time));
}

void Engine::Draw()
{
    glClearColor(1, 1,1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    sceneManager.GetScene()->Draw();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(ptr_window);

}

bool Engine::ShouldClose()
{
    return glfwWindowShouldClose(ptr_window);
}

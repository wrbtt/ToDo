#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ToDo.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>

void init_imgui(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    io.Fonts->AddFontFromFileTTF("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 24.0f);

    ImGui_ImplOpenGL3_DestroyFontsTexture();
    ImGui_ImplOpenGL3_CreateFontsTexture();
}

void create_ui(ToDoList& todo) {
    static bool darkTheme = false;
    ImVec4 bgColor = darkTheme ? ImVec4(0.1f, 0.1f, 0.1f, 0.1f)
                                : ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    ImVec4 textColor = darkTheme ? ImVec4(1.0f, 1.0f, 1.0f, 1.0f)
                                : ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    

    ImGui::SetNextWindowPos(ImVec2(0,0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar |
                                    ImGuiWindowFlags_NoResize | 
                                    ImGuiWindowFlags_NoMove |
                                    ImGuiWindowFlags_NoCollapse |
                                    ImGuiWindowFlags_NoScrollbar |
                                    ImGuiWindowFlags_NoScrollWithMouse;


    ImGui::PushStyleColor(ImGuiCol_WindowBg, bgColor);
    ImGui::PushStyleColor(ImGuiCol_Text, textColor);
    ImGui::Begin("ToDoList", nullptr, window_flags);

    // Theme 
    if (ImGui::Button("Toggle Theme")) darkTheme = !darkTheme; 
        ImGui::Separator();

    // Add Task
    static bool showInput = false;
    static char taskBuffer[128] = "";
    static int nextId = 1;

    if (ImGui::Button("Add Task")) showInput = true; 

        
    if (showInput) {
        ImGui::InputText("New Task", taskBuffer, sizeof(taskBuffer));
        if (ImGui::Button("Submit Task")) {
            todo.addTask(nextId++, taskBuffer);
            taskBuffer[0] = '\0';
            showInput = false; // скрываем поле после добавления
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) showInput = false; 
        
        ImGui::Separator();
    }

    // Кнопки удалить->подтвердить удаление
    static bool showInputId = false;
    static unsigned int removeId;

    if (ImGui::Button ("Remove Task")) showInputId = true; 


    if (showInputId) {
        ImGui::InputScalar("Task ID to remove", ImGuiDataType_U32, &removeId);
        if (ImGui::Button("Submit Remove")) todo.RemoveTask(removeId); 
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) showInputId = false;
        
        ImGui::Separator();
        }

   
    ImGui::Separator();
    // Вывод задач
    for(const auto& t : todo.getTask()) {
        ImGui::Text("%d -> %s", t.IdTask, t.textTask.c_str());
    }

    ImGui::End();
    ImGui::PopStyleColor(2);
    }   


void render_frame(ToDoList& todo) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    create_ui(todo);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void cleanup_imgui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

int main() {
    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(1600, 900, "Todo App", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;

        
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    init_imgui(window);

    bool show_demo_window = true;

    ToDoList todo;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0,0,display_w, display_h);
       
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Твоё окно
        create_ui(todo);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    cleanup_imgui();
    glfwDestroyWindow(window);
    glfwTerminate();
}

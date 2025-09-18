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
    static char taskBuffer[128] = "";
    static char updateTaskBuffer[128] = "";
    static unsigned int updateId = 0;
    static unsigned int removeId = 0;
    static int nextId = 1;
    static bool showInput = false;
    static bool showInputUpdate = false;
    static bool showInputId = false;

    // Цветовые настройки
    ImVec4 bgColor = darkTheme ? ImVec4(0.1f, 0.1f, 0.1f, 1.0f) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    ImVec4 textColor = darkTheme ? ImVec4(1.0f, 1.0f, 1.0f, 1.0f) : ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

    // Настройка окна
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar |
                                   ImGuiWindowFlags_NoResize |
                                   ImGuiWindowFlags_NoMove |
                                   ImGuiWindowFlags_NoCollapse |
                                   ImGuiWindowFlags_NoScrollbar |
                                   ImGuiWindowFlags_NoScrollWithMouse;

    // Стили
    ImGui::PushStyleColor(ImGuiCol_WindowBg, bgColor);
    ImGui::PushStyleColor(ImGuiCol_Text, textColor);
    
    // Основное окно
    if (ImGui::Begin("ToDoList", nullptr, window_flags)) {
        // Переключение темы
        if (ImGui::Button(darkTheme ? "Switch to Light Theme" : "Switch to Dark Theme")) {
            darkTheme = !darkTheme;
        }
        ImGui::Separator();

        // Добавление задачи
        if (ImGui::Button("Add Task")) {
            showInput = true;
            showInputUpdate = false;
            showInputId = false;
            taskBuffer[0] = '\0'; 
        }

        if (showInput) {
            ImGui::Text("Add New Task:");
            ImGui::InputText("##NewTask", taskBuffer, sizeof(taskBuffer));
            
            ImGui::BeginGroup();
            if (ImGui::Button("Submit", ImVec2(100, 0))) {
                if (strlen(taskBuffer) > 0) {
                    todo.addTask(nextId++, taskBuffer);
                    taskBuffer[0] = '\0';
                    showInput = false;
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(100, 0))) {
                showInput = false;
                taskBuffer[0] = '\0';
            }
            ImGui::EndGroup();
            ImGui::Separator();
        }

        // Обновление задачи
        if (ImGui::Button("Update Task")) {
            showInputUpdate = true;
            showInput = false;
            showInputId = false;
            updateTaskBuffer[0] = '\0';
            updateId = 0;
        }

        if (showInputUpdate) {
            ImGui::Text("Update Task:");
            ImGui::InputScalar("Task ID##Update", ImGuiDataType_U32, &updateId);
            ImGui::InputText("New Task Text##Update", updateTaskBuffer, sizeof(updateTaskBuffer));
            
            ImGui::BeginGroup();
            if (ImGui::Button("Submit Update", ImVec2(120, 0))) {
                if (updateId > 0 && strlen(updateTaskBuffer) > 0) {
                    todo.UpdateTask(updateId, updateTaskBuffer);
                    updateTaskBuffer[0] = '\0';
                    updateId = 0;
                    showInputUpdate = false;
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(100, 0))) {
                showInputUpdate = false;
                updateTaskBuffer[0] = '\0';
                updateId = 0;
            }
            ImGui::EndGroup();
            ImGui::Separator();
        }

        // Удаление задачи
        if (ImGui::Button("Remove Task")) {
            showInputId = true;
            showInput = false;
            showInputUpdate = false;
            removeId = 0;
        }

        if (showInputId) {
            ImGui::Text("Remove Task:");
            ImGui::InputScalar("Task ID##Remove", ImGuiDataType_U32, &removeId);
            
            ImGui::BeginGroup();
            if (ImGui::Button("Remove", ImVec2(100, 0))) {
                if (removeId > 0) {
                    todo.RemoveTask(removeId);
                    removeId = 0;
                    showInputId = false;
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(100, 0))) {
                showInputId = false;
                removeId = 0;
            }
            ImGui::EndGroup();
            ImGui::Separator();
        }

        // Отображение списка задач
        ImGui::Spacing();
        ImGui::Text("Tasks List:");
        ImGui::Separator();
        
        const auto& tasks = todo.getTask();
        if (tasks.empty()) {
            ImGui::Text("No tasks available.");
        } else {
            for (const auto& t : tasks) {
                ImGui::Text("%d. %s", t.IdTask, t.textTask.c_str());
            }
        }
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

        
        create_ui(todo);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    cleanup_imgui();
    glfwDestroyWindow(window);
    glfwTerminate();
}

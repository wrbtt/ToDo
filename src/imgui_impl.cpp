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

    // Шрифт 
    io.Fonts->AddFontFromFileTTF("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 24.0f);

    ImGui_ImplOpenGL3_DestroyFontsTexture();
    ImGui_ImplOpenGL3_CreateFontsTexture();
}

// Функция для окраски кнопок
void ColorButton() {
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.08f, 0.9f, 0.9f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.08f, 1.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.08f, 0.8f, 0.8f));
}

void create_ui(ToDoList& todo) {
    static bool darkTheme = false;
    static char taskBuffer[128] = "";
    static char updateTaskBuffer[128] = "";
    static unsigned int updateId = 0;
    static unsigned int removeId = 0;
    static bool showInput = false;
    static bool showInputUpdate = false;
    static bool showInputId = false;
    static int openPopupId = -1; 

    // Цветовые настройки Окна и Текста
    ImVec4 bgColor = darkTheme ? ImVec4(0.1f, 0.1f, 0.1f, 1.0f) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    ImVec4 textColor = darkTheme ? ImVec4(1.0f, 1.0f, 1.0f, 1.0f) : ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    ImVec4 childBgColor = darkTheme ? ImVec4(0.15f, 0.15f, 0.15f, 1.0f) : ImVec4(0.95f, 0.95f, 0.95f, 1.0f);

    // Цветовые настройки для Popup
    ImVec4 popupBgColor = darkTheme ? ImVec4(0.2f, 0.2f, 0.2f, 1.0f) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    ImVec4 popupTextColor = darkTheme ? ImVec4(1.0f, 1.0f, 1.0f, 1.0f) : ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    ImVec4 popupBorderColor = darkTheme ? ImVec4(0.4f, 0.4f, 0.4f, 1.0f) : ImVec4(0.7f, 0.7f, 0.7f, 1.0f);


    // Настройки окна 
    ImGui::SetNextWindowPos(ImVec2(0,0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar |
                                    ImGuiWindowFlags_NoResize |
                                    ImGuiWindowFlags_NoMove |
                                    ImGuiWindowFlags_NoCollapse;

    // Стили
    ImGui::PushStyleColor(ImGuiCol_WindowBg, bgColor);
    ImGui::PushStyleColor(ImGuiCol_Text, textColor);
    ImGui::PushStyleColor(ImGuiCol_ChildBg, childBgColor);

    // Стили для Popup
    ImGui::PushStyleColor(ImGuiCol_PopupBg, popupBgColor);
    ImGui::PushStyleColor(ImGuiCol_Text, popupTextColor);
    ImGui::PushStyleColor(ImGuiCol_Border, popupBorderColor);

    // Основное окно
    if (ImGui::Begin("ToDo", nullptr, window_flags)) {
        // Переключение темы - в верхней части окна
        ColorButton();
        if (ImGui::Button(darkTheme ? "Light Mode" : "Dark Mode")) {
            darkTheme = !darkTheme;
        }
        ImGui::PopStyleColor(3);
        ImGui::Separator();

        // Разделение на колонки
        ImGui::Columns(2, "", true);

        // Левая - управление
        ImGui::BeginChild("ControlPanel");
        ImGui::Text("Control");
        ImGui::Separator();

        // Добавление задачи
        ColorButton();
        if (ImGui::Button("Add Task", ImVec2(-1, 40))) {
            showInput = true;
            showInputUpdate = false;
            showInputId = false;
            taskBuffer[0] = '\0';
        }
        ImGui::PopStyleColor(3);

        if (showInput) {
            ImGui::Separator();
            ImGui::Spacing();
            ImGui::Text("New Task: ");
            ImGui::InputText("##NewTask", taskBuffer, sizeof(taskBuffer));

            ImGui::Spacing();
            ImGui::Columns(2, "", false);

            ColorButton();
            if (ImGui::Button("Add", ImVec2(-1, 30))) {
                if (strlen(taskBuffer) > 0) {
                    todo.addTask(0, taskBuffer);
                    taskBuffer[0] = '\0';
                    showInput = false;
                }
            }
            ImGui::PopStyleColor(3);
            ImGui::NextColumn();

            ColorButton();
            if (ImGui::Button("Cancel", ImVec2(-1, 30))) {
                showInput = false;
                taskBuffer[0] = '\0';
            }
            ImGui::PopStyleColor(3);

            ImGui::Columns(1);
            ImGui::Separator();
        }

        ImGui::Spacing();

        // Обновление задачи
        ColorButton();
        if (ImGui::Button("Update Task", ImVec2(-1, 40))) {
            showInputUpdate = true;
            showInput = false;
            showInputId = false;
            updateTaskBuffer[0] = '\0';
            updateId = 0;
        }
        ImGui::PopStyleColor(3);

        if (showInputUpdate) {
            ImGui::Separator();
            ImGui::Spacing();
            ImGui::Text("Update Task (ID): ");
            ImGui::InputScalar("##UpdateID", ImGuiDataType_U32, &updateId);
            ImGui::Text("New Text: ");
            ImGui::InputText("##UpdateText", updateTaskBuffer, sizeof(updateTaskBuffer));

            ImGui::Spacing();
            ImGui::Columns(2, "", false);

            ColorButton();
            if (ImGui::Button("Update", ImVec2(-1, 30))) {
                if (updateId > 0 && strlen(updateTaskBuffer) > 0) {
                    todo.UpdateTask(updateId, updateTaskBuffer);
                    updateTaskBuffer[0] = '\0';
                    updateId = 0;
                    showInputUpdate = false;
                }
            }
            ImGui::PopStyleColor(3);

            ImGui::NextColumn();

            ColorButton();
            if (ImGui::Button("Cancel", ImVec2(-1,30))) {
                showInputUpdate = false;
                updateTaskBuffer[0] = '\0';
                updateId = 0;
            }
            ImGui::PopStyleColor(3);

            ImGui::Columns(1);
            ImGui::Separator();
        }

        ImGui::Spacing();

        // Удаление задачи
        ColorButton();
        if (ImGui::Button("Remove Task", ImVec2(-1, 40))) {
            showInputId = true;
            showInput = false;
            showInputUpdate = false;
            removeId = 0;
        }
        ImGui::PopStyleColor(3);

        if (showInputId) {
            ImGui::Separator();
            ImGui::Spacing();
            ImGui::Text("Remove Task (ID): ");
            ImGui::InputScalar("##RemoveID", ImGuiDataType_U32, &removeId);

            ImGui::Spacing();
            ImGui::Columns(2, "", false);

            ColorButton();
            if (ImGui::Button("Remove", ImVec2(-1, 30))) {
                if (removeId > 0) {
                    todo.RemoveTask(removeId);
                    removeId = 0;
                    showInputId = false;
                }
            }
            ImGui::PopStyleColor(3);

            ImGui::NextColumn();

            ColorButton();
            if (ImGui::Button("Cancel", ImVec2(-1, 30))) {
                showInputId = false;
                removeId = 0;
            }
            ImGui::PopStyleColor(3);

            ImGui::Columns(1);
            ImGui::Separator();
        }

        ImGui::EndChild();

        ImGui::NextColumn();

        // Правая колонка - список
        ImGui::BeginChild("TaskListPanel");
        ImGui::Text("Task List");
        ImGui::Separator();

        const auto& tasks = todo.getTask();
        if (tasks.empty()) {
            ImGui::Text("No tasks available. Add a new Task!");
        } else {
            // Прокрутка область задач
            if (ImGui::BeginChild("TaskScroll", ImVec2(0,0), true, 
                ImGuiWindowFlags_AlwaysVerticalScrollbar)) {

                for(const auto& t : tasks) {
                    ImGui::PushID(t.IdTask);

                    ImGui::BeginGroup();
                    
                    // Отображение задачи
                    ImGui::TextWrapped("%d. %s", t.IdTask, t.textTask.c_str());
                    
                    ImGui::EndGroup();

                    
                    ImVec2 textSize = ImGui::GetItemRectSize();
                    
                    // Кнопка "..." - для действий с задачей
                    ImGui::SameLine(ImGui::GetContentRegionAvail().x - 30);
                    if (ImGui::SmallButton(("..." + std::to_string(t.IdTask)).c_str())) {
                        openPopupId = t.IdTask;
                        ImGui::OpenPopup(("task_popup_" + std::to_string(t.IdTask)).c_str());
                    }

                    // Popup меню для действий с задачей
                    if (ImGui::BeginPopup(("task_popup_" + std::to_string(t.IdTask)).c_str())) {

                        ImGui::PushStyleColor(ImGuiCol_Text, popupTextColor);

                        if (ImGui::MenuItem("Edit")) {
                            updateId = t.IdTask;
                            strncpy(updateTaskBuffer, t.textTask.c_str(), sizeof(updateTaskBuffer));
                            updateTaskBuffer[sizeof(updateTaskBuffer) - 1] = '\0';
                            showInputUpdate = true;
                            showInput = false;
                            showInputId = false;
                            ImGui::CloseCurrentPopup();
                        }

                        if (ImGui::MenuItem("Delete")) {
                            todo.RemoveTask(t.IdTask);
                            showInputUpdate = false;
                            ImGui::CloseCurrentPopup();
                        }
                        ImGui::PopStyleColor();
                        ImGui::EndPopup();
                    }

                    ImGui::PopID();
                    ImGui::Separator();
                }

                ImGui::EndChild(); // TaskScroll
            }
        }
        ImGui::EndChild(); // TaskListPanel

        ImGui::Columns(1); 
    }
    ImGui::End();
    ImGui::PopStyleColor(6);
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

    ToDoList todo;

    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0,0, display_w, display_h);


        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);

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
    return 0;
}
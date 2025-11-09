#include "ToDo.h"
#include <iostream>
#include <algorithm>
using namespace std;

unsigned int ToDoList :: addTask(unsigned int id, const string& text) {
   unsigned int newId;
    if (!freeIds.empty()) {
        newId = freeIds.front();
        freeIds.pop();
    } 
    else {
        newId = nextId++;
    }
    tasks.push_back(Task{newId, text});
    return newId;
} 

const vector<Task>& ToDoList :: getTask() const {
    return tasks;
}


// Метод для отладки(не используется в src/imgui_impl.cpp)
void ToDoList :: PrintTask() {
    for(const auto& it : tasks) {
        cout << it.IdTask << " -> " << it.textTask << endl;
    }
}

void ToDoList :: RemoveTask(unsigned int id) {
   auto removeTask = find_if(tasks.begin(), tasks.end(),
        [id](const Task& t) {return t.IdTask == id; });
    
    if (removeTask != tasks.end()) {
        freeIds.push(removeTask->IdTask);
        tasks.erase(removeTask);
    }
    else 
        // Вывод для отладки
        cerr << "Error: There is no task for the specified ID!"
            << endl;
}

void ToDoList :: UpdateTask(unsigned int id, const string& newText) {
    
    auto taskIt = find_if(tasks.begin(), tasks.end(), [id](const Task& t)
    {
        return t.IdTask == id;
    });
    
    if (taskIt != tasks.end()) {
        taskIt->textTask = newText;
    } 
    else {
        // Вывод для отладки
        cout << "Task with id: " << id << " not found." << endl;
    }
}
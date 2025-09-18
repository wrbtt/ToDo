#include "ToDo.h"
#include <iostream>
#include <algorithm>
using namespace std;

void ToDoList :: addTask(unsigned int id, const string& text) {
    task.emplace_back(id, text);
}

const vector<Task>& ToDoList :: getTask() const {
    return task;
}

void ToDoList :: PrintTask() {
    for(const auto& it : task) {
        cout << it.IdTask << " -> " << it.textTask << endl;
    }
}

void ToDoList :: RemoveTask(unsigned int id) {
   auto removeTask = find_if(task.begin(), task.end(),
        [id](const Task& t) {return t.IdTask == id; });
    
    if (removeTask != task.end())
        task.erase(removeTask);
    else 
        cerr << "Error: There is no task for the specified ID!"
            << endl;
}

void ToDoList :: UpdateTask(unsigned int id, const string& newText) {
    
    auto findTask = find_if(task.begin(), task.end(), [id](const Task& t)
    {
        return t.IdTask == id;
    });
    
    if (findTask != task.end()) {
        findTask->textTask = newText;
        cout << "Task updated!" << endl;
    } 
    else {
        cout << "Task with id: " << id << " not found." << endl;
    }
}
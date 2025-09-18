#pragma once
#include <string>
#include <vector>

using namespace std;

struct Task {
    string textTask;
    unsigned int IdTask;

    Task(unsigned int id, const string& text) 
        :IdTask(id), textTask(text){}
};

class ToDoList {
    vector<Task> task;

    public: 
        void addTask(unsigned int id, const string& text);
        void PrintTask();
        void RemoveTask(unsigned int id);
        void UpdateTask();
        const vector<Task>& getTask() const;
};


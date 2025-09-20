#pragma once
#include <string>
#include <vector>
#include <queue>

using namespace std;

struct Task {
    string textTask;
    unsigned int IdTask;

    Task(unsigned int id, const string& text) 
        :IdTask(id), textTask(text){}
};

class ToDoList {
    vector<Task> tasks;
    queue<unsigned int> freeIds;
    unsigned int nextId = 1;


    public: 
        unsigned int addTask(unsigned int id, const string& text);
        void PrintTask();
        void RemoveTask(unsigned int id);
        void UpdateTask(unsigned int id, const string& newTask);
        const vector<Task>& getTask() const;
};
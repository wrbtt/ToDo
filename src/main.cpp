#include "ToDo.h"

int main() {
    ToDoList task;
    task.addTask(3, "go to work");
    task.addTask(4, "Sleep");
    task.PrintTask();

    task.UpdateTask();
    task.PrintTask();

    return 0;
}


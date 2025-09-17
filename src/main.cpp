#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct Task {
    string textTask;
    int IdTask;

    Task(int id, const string& text)
        :textTask(text), IdTask(id){}
};

class ToDoList {
    vector<Task> task;

    public:
        void addTask(int id, const string& text) {
            task.emplace_back(id, text);
        }

       void PrintTask() {
            for(const auto& it : task) {
                cout << it.IdTask << " -> " << it.textTask
                    << endl;
            }
       }

       void RemoveTask(int id) {
            if (id < task.size())
                task.erase(task.begin() + id);
            else
                cerr << "Error: There is no task for the specified ID!" << endl;
        }

        void UpdateTask() {
            cout << "Input task id: ";
            int id;
            cin >> id; 
            cin.ignore();

            auto findTask = find_if(task.begin(), task.end(), [id](const Task& t) {
                return t.IdTask == id;
            }); 

            if (findTask != task.end()) {
                cout << "Input task text: ";
                string inputNewText;
                getline(cin, inputNewText);

                findTask->textTask = inputNewText;
                cout << "Task updated!" << endl;

            } else {
                cout << "Task with id: " << id << " not found." << endl;
            }
        }
};


int main() {
    ToDoList task;
    task.addTask(3, "go to work");
    task.addTask(4, "Sleep");
    task.PrintTask();

    task.UpdateTask();
    task.PrintTask();

    return 0;
}



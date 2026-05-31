#include "Task_funcs.h"
#include "Base_modules.h"
#include <limits>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cctype>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif


Actions::Actions() {}

void Actions::delimiter(int count) {
    std::string line = "";
    std::cout << "\n";
    while (count--) {
        std::cout << "---";
    }
    std::cout << std::endl;
}

std::string Actions::get_password() {
    std::string password;

#if defined(_WIN32) || defined(_WIN64)
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & ~ENABLE_ECHO_INPUT);

    std::cin >> password;

    SetConsoleMode(hStdin, mode);
#else
    termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    termios newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    std::cin >> password;

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif

    return password;
}

httplib::Headers Actions::user_reg(httplib::Client& cli) {
    std::string username;

    while (true) {
        delimiter(20);
        std::cout << "Please enter your username: ";
        std::cin >> username;
        std::cout << std::endl << "Please enter your password: ";
        std::string password = get_password();
        std::cout << std::endl;

        httplib::Headers res = server.reg(cli, username, password);
        if (!res.empty()) {
            return res;
        } else {
            std::cout <<
            "Error: login is taken.\n"
            "Please, enter new login."
            << std::endl;
        }
    }
}

httplib::Headers Actions::user_login(httplib::Client& cli) {
    std::string username;

    while (true) {
        delimiter();
        std::cout << "Please enter your username: ";
        std::cin >> username;
        std::cout << std::endl << "Please enter your password: ";
        std::string password = get_password();
        std::cout << std::endl;

        httplib::Headers res = server.login(cli, username, password);
        if (!res.empty()) {
            return res;
        } else {
            std::cout <<
            "Error: login or passwrod was wrong.\n"
            "Please, repeat."
            << std::endl;
        }
    }
}

void Actions::task_delete(httplib::Client& cli, httplib::Headers headers) {
    std::string answer;
    int id = -1;

    while (true) {
        std::vector<ActionWithServer::Variable> tasks = server.get_tasks(cli, headers);
    
        delimiter();
        print_tasks(tasks);
        std::cout << std::endl << "If you want to cancel delete, enter less than or equal to 0.\n"
        << "Please enter id for delete task: ";
        std::cin >> id;
        if (id < 1) {
            return;
        } else if (!id_check(tasks, id)) {
            std::cout << std::endl << "This id(" << id << ") doens't exist." << std::endl;
            continue;
        }
        int server_id = tasks[id-1].id;
        std::cout << std::endl << "Are you sure? Yes/No: ";
        std::cin >> answer;
        std::cout << std::endl;
        answer = lower(answer);
        if (answer == "yes") {
            server.remove_task(cli, headers, server_id);
        } else {
            std::cout << std::endl << "OK. You can exit, enter less than or equal to 0. Or continue to delete." << std::endl;
        }
    }
}

std::string Actions::lower(std::string text) {
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    return text;
}

bool Actions::id_check(const std::vector<ActionWithServer::Variable>& tasks, int id) {
    if (id < 1 || static_cast<size_t>(id - 1) >= tasks.size()) {
        return false;
    }
    return true;
}

void Actions::print_tasks(const std::vector<ActionWithServer::Variable>& tasks) {
    std::cout << "Your tasks:" << std::endl;
        for (size_t i = 0; i < tasks.size(); i++) {
            std::cout << "Id: " << i+1
            << "\nTask: " << tasks[i].task
            << "\nDeadline: " << tasks[i].deadline
            << "\nComplete: " << tasks[i].complete;
            delimiter();
        }
}

























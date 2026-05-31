#include <httplib.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>

struct Variable {
    int id;
    std::string login;
    std::string task;
    std::string deadline;
    bool complete;
};
    
httplib::Headers login(httplib::Client& cli, std::string login, std::string password){

    std::string user_login = "username=" + login;
    std::string user_password = "&password=" + password;
    std::string user = user_login + user_password;

    auto response_token = cli.Post("/login", user, "application/x-www-form-urlencoded");

    if (response_token)
        if (response_token->status == 200) {
            std::string pizdec = nlohmann::json::parse(response_token->body)["access_token"];
            std::string token = "Bearer " + pizdec;
            std::cout << "Token was accepted\n";
            return {{"Authorization", token}};
        } else { 
            std::cout << "Server error: " << response_token->status << std::endl;         
        }
    else {
        auto error_response = response_token.error();
        std::cout << "Network error. Code: " << static_cast<int>(error_response) << std::endl;
    }
    return httplib::Headers();
}




httplib::Headers reg(httplib::Client& cli, std::string login, std::string password){

    nlohmann::json json_body;
    json_body["login"] = login;
    json_body["password"] = password;

    std::string user = json_body.dump();

    auto response_token = cli.Post("/register", user, "application/json");

    if (response_token)
        if (response_token->status == 201) {
            std::string pizdec = nlohmann::json::parse(response_token->body)["token"];
            std::string token = "Bearer " + pizdec;
            std::cout << "Token was accepted\n";
            return {{"Authorization", token}};
        } else { 
            std::cout << "Server error: " << response_token->status << std::endl;         
        }
    else {
        auto error_response = response_token.error();
        std::cout << "Network error. Code: " << static_cast<int>(error_response) << std::endl;
    }
    return httplib::Headers();
}



std::vector<Variable> get_tasks(httplib::Client& cli, httplib::Headers headers){
    auto tasks = cli.Get("/tasks", headers);

    std::vector<Variable> task_list;

    if (tasks)
        if (tasks->status == 200) {
            auto res = nlohmann::json::parse(tasks->body)["tasks"];
            for (const auto& i : res) {
                Variable n;
                n.id = i["id"];
                n.login = i["login"];
                n.task = i["task"];
                n.deadline = i["deadline"];
                n.complete = i["complete"];
                task_list.push_back(n);
            }
        } else { 
            std::cout << "Server error: " << tasks->status << std::endl;         
        }
    else {
        auto error_response = tasks.error();
        std::cout << "Network error. Code: " << static_cast<int>(error_response) << std::endl;
    }
    return task_list;
}


void remove_task(httplib::Client& cli, httplib::Headers headers, int id){
    std::string path = "/tasks/remove?id=" + std::to_string(id);

    auto rem = cli.Delete(path, headers);

    if (rem)
        if (rem->status == 200) {
            std::cout << "Task was deleted";
        } else { 
            std::cout << "Server error: " << rem->status << std::endl;         
        }
    else {
        auto error_response = rem.error();
        std::cout << "Network error. Code: " << static_cast<int>(error_response) << std::endl;
    }
}

void add_task(httplib::Client& cli, httplib::Headers headers, int id, std::string login, std::string task, std::string deadline, bool complete){

    nlohmann::json add;
    add["task"] = task;
    add["deadline"] = deadline;
    add["complete"] = complete;

    std::string body = add.dump();

    auto errors = cli.Post("/tasks/add", headers, body, "application/json");

    if (errors)
        if (errors->status == 201) {
            std::cout << "Task was added";
        } else { 
            std::cout << "Server error: " << errors->status << std::endl;         
        }
    else {
        auto error_response = errors.error();
        std::cout << "Network error. Code: " << static_cast<int>(error_response) << std::endl;
    }
}

void upd_task(httplib::Client& cli, httplib::Headers headers, int id, std::string login, std::string task, std::string deadline, bool complete){

    nlohmann::json upd;
    upd["id"] = id;
    upd["login"] = login;
    upd["task"] = task;
    upd["deadline"] = deadline;
    upd["complete"] = complete;

    std::string body = upd.dump();

    auto errors = cli.Put("/tasks/update", headers, body, "application/json");

    if (errors)
        if (errors->status == 200) {
            std::cout << "Task was updated";
        } else { 
            std::cout << "Server error: " << errors->status << std::endl;         
        }
    else {
        auto error_response = errors.error();
        std::cout << "Network error. Code: " << static_cast<int>(error_response) << std::endl;
    }
}


int main() {
    httplib::Client cli("http://127.0.0.1:8000");
    httplib::Headers headers;
    bool is_authorized = false;
    std::string current_user = "";

    while (true) {
        if (!is_authorized) {
            std::cout << "\n--- Меню ---\n";
            std::cout << "1. Регистрация\n";
            std::cout << "2. Вход\n";
            std::cout << "3. Выход из программы\n";
            std::cout << "Выберите действие: ";
            
            int choice;
            std::cin >> choice;

            if (choice == 3) break;

            std::string user, pass;
            switch (choice) {
                case 1:
                    std::cout << "Введите логин: "; std::cin >> user;
                    std::cout << "Введите пароль: "; std::cin >> pass;
                    reg(cli, user, pass);
                    break;
                case 2:
                    std::cout << "Введите логин: "; std::cin >> user;
                    std::cout << "Введите пароль: "; std::cin >> pass;
                    headers = login(cli, user, pass);
                    
                    // Если заголовки не пустые, значит токен получен успешно
                    if (!headers.empty()) {
                        is_authorized = true;
                        current_user = user;
                    }
                    break;
                default:
                    std::cout << "Неверный пункт меню.\n";
            }
        } else {

            std::cout << "\n--- Управление задачами для " << current_user << " ---\n";
            std::cout << "1. Показать все задачи\n";
            std::cout << "2. Добавить задачу\n";
            std::cout << "3. Изменить задачу\n";
            std::cout << "4. Выйти из аккаунта\n";
            std::cout << "5. Удалить задачу\n"; // Добавлен новый пункт
            std::cout << "Выберите действие: ";

            int choice;
            std::cin >> choice;

            if (choice == 4) {
                is_authorized = false;
                headers.clear();
                continue;
            }

            if (choice == 1) {
                std::vector<Variable> tasks = get_tasks(cli, headers);
                if (tasks.empty()) {
                    std::cout << "Список задач пуст или произошла ошибка.\n";
                } else {
                    std::cout << "\n--- Список задач ---\n";
                    for (const auto& task : tasks) {
                        std::cout << "ID: " << task.id 
                                  << " | Задача: " << task.task 
                                  << " | Дедлайн: " << task.deadline 
                                  << " | Статус: " << (task.complete ? "Выполнено" : "В процессе") 
                                  << "\n";
                    }
                }
            } else if (choice == 2) {
                std::string task_text, deadline;
                std::cout << "Введите описание задачи: ";
                std::cin.ignore(); // Очистка буфера после ввода choice
                std::getline(std::cin, task_text);
                std::cout << "Введите дедлайн: ";
                std::getline(std::cin, deadline);

                // id передается 0, так как на сервере автоинкремент (PRIMARY KEY AUTOINCREMENT)
                add_task(cli, headers, 0, current_user, task_text, deadline, false);
                std::cout << "\n";
            } 
            else if (choice == 3) {
                int id;
                std::string task_text, deadline;
                std::string status_input;
                bool complete = false;

                std::cout << "Введите ID задачи для изменения: ";
                std::cin >> id;
                std::cout << "Введите новое описание задачи: ";
                std::cin.ignore();
                std::getline(std::cin, task_text);
                std::cout << "Введите новый дедлайн: ";
                std::getline(std::cin, deadline);
                std::cout << "Задача выполнена? (1 - да, 0 - нет): ";
                std::cin >> status_input;
                if (status_input == "1") complete = true;

                upd_task(cli, headers, id, current_user, task_text, deadline, complete);
                std::cout << "\n";
            } else if (choice == 5) { // Логика для удаления задачи
                int id;
                std::cout << "Введите ID задачи для удаления: ";
                std::cin >> id;
                remove_task(cli, headers, id);
                std::cout << "\n";
            }
            else {
                std::cout << "Неверный пункт меню.\n";
            }
        }
        }
    return 0;
}
    





#ifndef BASE_MODULES_H
#define BASE_MODULES_H
#include <httplib.h>
#include "Task_funcs.h"
#include <vector>
#include <iostream>
#include <nlohmann/json.hpp>

class Actions {
    private:
        ActionWithServer server;
    public:
        Actions();
        
        std::string get_password();
        httplib::Headers user_reg(httplib::Client& cli);
        void delimiter(int count = 20);
        httplib::Headers user_login(httplib::Client& cli);
        void task_delete(httplib::Client& cli, httplib::Headers headers);
        std::string lower(std::string text);
        bool id_check(const std::vector<ActionWithServer::Variable>& tasks, int id);
        void print_tasks(const std::vector<ActionWithServer::Variable>& tasks);
    
    };

#endif

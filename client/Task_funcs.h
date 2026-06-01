#ifndef TASK_FUNCS_H
#define TASK_FUNCS_H
#include <httplib.h>
#include <vector>

class ActionWithServer{
private:
    int id;
    std::string user_login;
    std::string task;
    std::string deadline;
    bool complete;
public:
    struct Variable {
        int id;
        std::string login;
        std::string task;
        std::string deadline;
        bool complete;
    };

    httplib::Headers login(httplib::Client& cli,
        std::string login,
        std::string password);

    httplib::Headers reg(httplib::Client& cli,
        std::string login,
        std::string password);

    bool remove_task(httplib::Client& cli,
        httplib::Headers headers,
        int id);

    bool add_task(httplib::Client& cli,
        httplib::Headers headers,
        std::string task,
        std::string deadline,
        bool complete = false);

    void upd_task(httplib::Client& cli,
        httplib::Headers headers,
        int id,
        std::string task,
        std::string deadline,
        bool complete);

    std::vector<Variable> get_tasks(httplib::Client& cli,
        httplib::Headers headers);


};


#endif
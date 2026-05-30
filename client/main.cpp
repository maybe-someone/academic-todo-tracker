#include <httplib.h>
#include <iostream>
#include <nlohmann/json.hpp>

int main() {


    // auto response_token = cli.Post("/register", user, "application/x-www-form-urlencoded");
    httplib::Headers login(std::string login, std::string password){
        httplib::Client cli("http://localhost:8080");

        std::string user_login = "username=" + login;
        std::string user_password = "&password=" + password;
        std::string user = user_login + user_password;

        auto response_token = cli.Post("/login", user, "application/x-www-form-urlencoded");

        if (response_token)
            if (response_token->status == 200) {
                std::string token = "Bearer " + nlohmann::json::parse(response_token->body)["access_token"];
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

    

    httplib::Headers headers = {
        token}
    };

    std::string task_info = "task=" + task + "&deadline=" + deadline + "&compete=" + compete;

    auto task_add = cli.Post("/tasks/add", headers, task_info, "application/x-www-form-urlencoded");



}
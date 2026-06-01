#include "Task_funcs.h"
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <vector>


httplib::Headers ActionWithServer::reg(httplib::Client& cli, std::string login, std::string password){

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

httplib::Headers ActionWithServer::login(httplib::Client& cli, std::string login, std::string password){

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

bool ActionWithServer::remove_task(httplib::Client& cli, httplib::Headers headers, int id){
	std::string path = "/tasks/remove?id=" + std::to_string(id);

	auto rem = cli.Delete(path, headers);

	if (rem)
		if (rem->status == 200) {
			std::cout << "Task was deleted";
			return true;
		} else { 
			std::cout << "Server error: " << rem->status << std::endl;		 
		}
	else {
		auto error_response = rem.error();
		std::cout << "Network error. Code: " << static_cast<int>(error_response) << std::endl;
	}
	return false;
}

bool ActionWithServer::add_task(httplib::Client& cli, httplib::Headers headers, std::string task, std::string deadline, bool complete){

	nlohmann::json add;
	add["task"] = task;
	add["deadline"] = deadline;
	add["complete"] = complete;

	std::string body = add.dump();

	auto errors = cli.Post("/tasks/add", headers, body, "application/json");

	if (errors)
		if (errors->status == 201) {
			std::cout << "Task was added";
			return true;
		} else { 
			std::cout << "Server error: " << errors->status << std::endl;		 
		}
	else {
		auto error_response = errors.error();
		std::cout << "Network error. Code: " << static_cast<int>(error_response) << std::endl;
	}
	return false;
}

void ActionWithServer::upd_task(httplib::Client& cli, httplib::Headers headers, int id, std::string task, std::string deadline, bool complete){

	nlohmann::json upd;
	upd["id"] = id;
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

std::vector<ActionWithServer::Variable> ActionWithServer::get_tasks(httplib::Client& cli, httplib::Headers headers){
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



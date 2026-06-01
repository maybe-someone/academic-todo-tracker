#ifndef BASE_MODULES_H
#define BASE_MODULES_H
#include <httplib.h>
#include "Task_funcs.h"
#include <vector>
#include <iostream>
#include <nlohmann/json.hpp>

class Actions {
	private:
		
	public:
		Actions();
		ActionWithServer server;

		httplib::Headers user_reg(httplib::Client& cli);
		httplib::Headers user_login(httplib::Client& cli);
		void print_tasks(const std::vector<ActionWithServer::Variable>& tasks);
		void task_add(httplib::Client& cli, httplib::Headers headers);
		void task_delete(httplib::Client& cli, httplib::Headers headers);
		void task_upd(httplib::Client& cli, httplib::Headers headers);
	};

#endif

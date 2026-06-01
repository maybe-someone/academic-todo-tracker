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


Actions::Actions () {};

namespace {
	void delimiter(int count = 20) {
		std::cout << "\n";
		while (count--) {
			std::cout << "---";
		}
		std::cout << std::endl;
	}
	
	bool question(std::string_view text, std::string& input, int set, std::string_view current_value = "") {
		delimiter();
		std::cout << "\nIf you want canceling, enter 'STOP!'.\n";
		if (!current_value.empty()) {
			std::cout << "[Current " << text << ": " << current_value << " (Press Enter to keep)]\n";
		}
		if (set == 1) {
			std::cout << "Please enter " << text << " of task: ";
		} else {
			std::cout << "If your task is " << text << ", enter 'Yes' or just ignore: ";
		}
		std::getline(std::cin, input);
		return (input == "STOP!");
	}

	std::string lower(std::string text) {
		std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) {
			return std::tolower(c);
		});
		return text;
	}

bool id_check(const std::vector<ActionWithServer::Variable>& tasks, int id) {
	if (id < 1 || static_cast<size_t>(id - 1) >= tasks.size()) {
		return false;
	}
	return true;
}

	std::string get_password() {
		std::string password;

		#if defined(_WIN32) || defined(_WIN64)
			HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
			DWORD mode = 0;
			GetConsoleMode(hStdin, &mode);
			SetConsoleMode(hStdin, mode & ~ENABLE_ECHO_INPUT);
			std::getline(std::cin, password);
			SetConsoleMode(hStdin, mode);
		#else
			termios oldt;
			tcgetattr(STDIN_FILENO, &oldt);
			termios newt = oldt;
			newt.c_lflag &= ~ECHO;
			tcsetattr(STDIN_FILENO, TCSANOW, &newt);
			std::getline(std::cin, password);
			tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
		#endif

		return password;
	}
}

httplib::Headers Actions::user_reg(httplib::Client& cli) {
	std::string username;
	while (true) {
		delimiter();
		std::cout << "Please enter your username: ";
		std::getline(std::cin, username);
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
		std::getline(std::cin, username);
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
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		if (id < 1) {
			return;
		} else if (!id_check(tasks, id)) {
			std::cout << std::endl << "This id(" << id << ") doens't exist." << std::endl;
			continue;
		}
		int server_id = tasks[id-1].id;
		std::cout << std::endl << "Are you sure? Yes/No: ";
		std::getline(std::cin, answer);
		std::cout << std::endl;
		answer = lower(answer);
		if (answer == "yes") {
			server.remove_task(cli, headers, server_id);
		}
		std::cout << std::endl << "If you want stoping to delete, enter 'No': ";
		std::getline(std::cin, answer);
		std::cout << std::endl;
		answer = lower(answer);
		if (answer == "no") {
			return;
		}
	}
}

void Actions::task_add(httplib::Client& cli, httplib::Headers headers) {
	std::string task;
	std::string deadline;
	std::string complete_text;
	bool complete;
	std::string answer;
	if (std::cin.peek() == '\n') {
		std::cin.ignore();
	}
	while (true) {
		if (question("text", task, 1) || question("deadline", deadline, 1) || question("complete", complete_text, 0)) {
			return;
		}
		complete = lower(complete_text) == "yes";
		server.add_task(cli, headers, task, deadline, complete);
		std::cout << std::endl << "If you want stoping to add, enter 'No': ";
		std::getline(std::cin, answer);
		std::cout << std::endl;
		answer = lower(answer);
		if (answer == "no") {
			return;
		}
	}
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

void Actions::task_upd(httplib::Client& cli, httplib::Headers headers) {
	std::string task;
	std::string deadline;
	std::string complete_text;
	bool complete;
	std::string answer;
	int id;
	if (std::cin.peek() == '\n') {
		std::cin.ignore();
	}
	while (true) {
		std::vector<ActionWithServer::Variable> tasks = server.get_tasks(cli, headers);
		delimiter();
		print_tasks(tasks);
		std::cout << "Enter task's id for update: ";
		if (!(std::cin >> id)) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		if (id < 1) {
			return;
		} else if (!id_check(tasks, id)) {
			std::cout << std::endl << "This id(" << id << ") doens't exist." << std::endl;
			continue;
		}
		const auto& current_task = tasks[id - 1];
		int server_id = current_task.id;
		task = current_task.task;
		deadline = current_task.deadline;
		complete_text = current_task.complete ? "yes" : "no";
		std::string temp_task, temp_deadline, temp_complete;
		std::cout << "Press Enter to keep current";
		if (question("text", temp_task, 1, task)) return;
		if (!temp_task.empty()) task = temp_task;
		if (question("deadline", temp_deadline, 1, deadline)) return;
		if (!temp_deadline.empty()) deadline = temp_deadline;
		if (question("complete", temp_complete, 0, complete_text)) return;
		if (!temp_complete.empty()) complete_text = temp_complete;
		complete = lower(complete_text) == "yes";
		std::cout << std::endl << "Are you sure? Yes/No: ";
		std::getline(std::cin, answer);
		std::cout << std::endl;
		answer = lower(answer);
		if (answer == "yes") {
			server.upd_task(cli, headers, server_id, task, deadline, complete);
		}
		std::cout << std::endl << "If you want stoping to update, enter 'No': ";
		std::getline(std::cin, answer);
		std::cout << std::endl;
		answer = lower(answer);
		if (answer == "no") {
			return;
		}


	}
}


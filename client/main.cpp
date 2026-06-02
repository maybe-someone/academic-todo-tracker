#include "Task_funcs.h"
#include "Base_modules.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {


	std::string server_url = "http://localhost:8000";
	
	if (argc > 1) {
		server_url = argv[1];
	}

	std::cout << "Connecting to server: " << server_url << std::endl;
	httplib::Client cli(server_url);
	
	Actions actions;
	httplib::Headers headers;

	std::cout << "=== Welcome to Task Manager ===" << std::endl;

	while (true) {
		std::cout << "\n1. Register\n2. Login\n3. Exit\nChoose option: ";
		int auth_choice = 0;
		if (!(std::cin >> auth_choice)) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Invalid input. Please enter a number." << std::endl;
			continue;
		}
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		if (auth_choice == 1) {
			headers = actions.user_reg(cli);
			if (!headers.empty()) {
				std::cout << "Registration successful and logged in!" << std::endl;
				break;
			}
		} else if (auth_choice == 2) {
			headers = actions.user_login(cli);
			if (!headers.empty()) {
				std::cout << "Login successful!" << std::endl;
				break;
			}
		} else if (auth_choice == 3) {
			std::cout << "Goodbye!" << std::endl;
			return 0;
		} else {
			std::cout << "Unknown option. Please try again." << std::endl;
		}
	}

	while (true) {
		std::cout << "\n=== Main Menu ===\n"
				  << "1. Show tasks\n"
				  << "2. Add task\n"
				  << "3. Update task\n"
				  << "4. Delete task\n"
				  << "5. Exit\n"
				  << "Choose action: ";

		int menu_choice = 0;
		if (!(std::cin >> menu_choice)) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Invalid input. Please enter a number." << std::endl;
			continue;
		}
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		if (menu_choice == 1) {
			auto tasks = actions.server.get_tasks(cli, headers);
			actions.print_tasks(tasks);
		} else if (menu_choice == 2) {
			actions.task_add(cli, headers);
		} else if (menu_choice == 3) {
			actions.task_upd(cli, headers);
		} else if (menu_choice == 4) {
			actions.task_delete(cli, headers);
		} else if (menu_choice == 5) {
			std::cout << "Goodbye!" << std::endl;
			break;
		} else {
			std::cout << "Unknown option. Please try again." << std::endl;
		}
	}

	return 0;
}
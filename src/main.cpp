#include "../include/Server.hpp"

int main(int argc, char* argv[]) {
	try {
		int port = 6667;
		if (argc != 3) {
			throw std::runtime_error("Invalid argument number.");
			return (0);
		}

		port = std::atoi(argv[1]);
		std::string password = argv[2];

		if (port <= 1023 || port > 7000) {
			throw std::runtime_error("Non common port number. Suggested range 1024 ~ 7000");
		}
		if (password.empty()) {
			throw std::runtime_error("Password cannot be empty");
		}
		Server server(port, password);
		server.run();
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	std::cout << "very gracefully" << std::endl;
	return 0;
}

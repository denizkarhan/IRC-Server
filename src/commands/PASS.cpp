#include "../../headers/Server.hpp"

void	Server::pass(int fd, std::vector<std::string> token) {
	// std::cout << "PASS Function started\n";

	std::string msg = "ERROR :Closing Link: [client IP address] (Incorrect password)\r\n";
	if (token.size() < 2) // Sadece PASS
		msg = "Error: Password information is missing\r\n";
	if (std::strstr(this->getPassword(), token[1].c_str())) { // Şifreyi bilen birisi server'a girebilir ileride status durumu olacak, şimdilik 1 ise server'a girmiştir.
		_clients[fd]->setStatus(1);
		return ;
	}
	send(fd, msg.c_str(), msg.size(), 0);
}

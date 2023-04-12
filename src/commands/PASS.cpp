#include "../../headers/Server.hpp"

void	Server::pass(int fd, std::vector<std::string> token) {
	std::string msg = "Pass success";
	if (token.size() < 2)
		msg = "Error: Password information is missing\r\n";
	if (this->getPassword() == token[1]) {
		_clients[fd]->setStatus(1);
		return ;
	}
	ft_write(fd, msg);
}

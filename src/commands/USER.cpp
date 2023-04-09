#include "../../headers/Server.hpp"

void	Server::user(int fd, std::vector<std::string> token) {
	// std::cout << "USER Function started\n";
	// /USER <username> <hostname> <servername> <realname>

	std::string msg;

	if (token.size() != 5)
	{
		msg = "5 Parametre girmeniz gerekmekte!\r\n";
		send(fd, msg.c_str(), msg.size(), 0);
		return;
	}

	_clients[fd]->setUserName(token[1]);
	_clients[fd]->setRealName(token[4].substr(token[4][0] == ':', token[4].size())); // realnamein başında genelde : oluyor onu keserek aldım.
}

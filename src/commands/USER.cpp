#include "../../headers/Server.hpp"

void	Server::user(int fd, std::vector<std::string> token) {
	// /USER <username> <hostname> <servername> <realname>

	std::string msg;

	if (token.size() != 5)
	{
		msg = ":" + _clients[fd]->getPrefixName() + " 461 " + _clients[fd]->getNickName() + " USER :Not enough parameters";
		ft_write(fd, msg);
		return;
	}

	_clients[fd]->setUserName(token[1]);
	_clients[fd]->setRealName(token[4].substr(token[4][0] == ':', token[4].size()));
	msg = ":"  +_clients[fd]->getPrefixName() + " 001 " + _clients[fd]->getNickName() + " :Welcome to FT_IRC Made by Mhaksal and Dkarhan";
	ft_write(fd, msg);
}

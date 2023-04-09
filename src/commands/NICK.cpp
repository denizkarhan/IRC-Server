#include "../../headers/Server.hpp"

void	Server::nick(int fd, std::vector<std::string> token) {
	// std::cout << "NICK Function started\n";
	std::string msg;

	if (token.empty() || token[0].empty() || token[1].empty())
	{
		msg = "User with alias" + token[1] + "not found\r\n";
		send(fd, msg.c_str(), msg.size(), 0);
		return;
	}

	std::map<int, Client*>::iterator it;
	for (it = _clients.begin() ; it != _clients.end() ; it++)
	{
		if (it->second->getNickName() == _clients[fd]->getNickName())
		{
			if (token[1] == _clients[fd]->getNickName())
			{
				msg = "Değiştirmek istediğiniz nick şimdiki isminizden farklı olmalıdır!\r\n";
				send(fd, msg.c_str(), msg.size(), 0);
			}
			else
			{
				msg = "[" + _clients[fd]->getNickName() + "] yeni nick: [" + token[1] + "]\r\n";
				send(fd, msg.c_str(), msg.size(), 0);
				it->second->setNickName(token[1]);
			}
			break;
		}
	}
}
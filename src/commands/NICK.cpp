#include "../../headers/Server.hpp"

void	Server::nick(int fd, std::vector<std::string> token) {
	std::string msg;

	if (token.empty() || token[0].empty() || token[1].empty())
	{
		msg = "ERROR :User with alias" + token[1] + "not found";
		ft_write(fd, msg);
		return;
	}

	if (token[1] == _clients[fd]->getNickName())
	{
		msg = "ERROR :Nick name is previously used";
		ft_write(fd, msg);
		return ;
	}
	
	std::map<int, Client*>::iterator it;
	for (it = _clients.begin() ; it != _clients.end() ; it++)
	{
		if (it->second->getFd() != fd && token[1] == it->second->getNickName())
		{
			_clients[fd]->setStatus(0);
			return ;
		}
	}
	msg = ":" + _clients[fd]->getPrefixName() + " NICK " + token[1];
	if (!_clients[fd]->getNickName().empty())
		ft_write(fd, msg);
	_clients[fd]->setNickName(token[1]);
}
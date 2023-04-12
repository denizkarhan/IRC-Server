#include "../../headers/Server.hpp"

void	Server::nick(int fd, std::vector<std::string> token) {
	std::string msg;

	if (token.empty() || token[0].empty() || token[1].empty())
	{
		msg = "ERROR :User with alias" + token[1] + "not found";
		ft_write(fd, msg);
		return;
	}

	std::map<int, Client*>::iterator it;
	for (it = _clients.begin() ; it != _clients.end() ; it++)
	{
		if (it->second->getNickName() == _clients[fd]->getNickName())
		{
			if (token[1] == _clients[fd]->getNickName())
			{
				msg = "The nickname different from your current nickname!";
				ft_write(fd, msg);
			}
			else
			{
				msg = ":" + _clients[fd]->getPrefixName() + " NICK " + token[1];
				it->second->setNickName(token[1]);
				ft_write(fd, msg);
			}
			break;
		}
	}
}
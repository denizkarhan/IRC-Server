#include "../../headers/Server.hpp"

void	Server::ping(int fd, std::vector<std::string> token)
{
    if (token.empty() or token.size() < 2) {
		_clients[fd]->clientMsgSender(fd, ERR_NEEDMOREPARAMS(_clients[fd]->getNickName(), "PING"));
		return ;
	}
	std::string msg = "PONG :" + token[1];
	std::cout << token[1] << std::endl;
	ft_write(fd, msg);
}


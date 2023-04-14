#include "../../headers/Server.hpp"

void	Server::ping(int fd, std::vector<std::string> token)
{
    if (token.empty() or token.size() < 2) {
		_clients[fd]->clientMsgSender(fd, ERR_NEEDMOREPARAMS(_clients[fd]->getNickName(), "PING"));
		return ;
	}
	std::string msg = RPL_PING(_clients[fd]->getPrefixName(), token[1]);
	ft_write(fd, msg);
}

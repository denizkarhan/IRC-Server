#include "../../headers/Server.hpp"

void    Server::broadcastNotice(const std::vector<Client *> &clientList, std::string msg, int excludeFd, std::string channelName) {
    for (size_t i = 0; i < clientList.size(); i++)
    {
        if (clientList[i]->getFd() == excludeFd)
            continue ;
        ft_write(clientList[i]->getFd(), ":" + _clients[excludeFd]->getPrefixName() + " NOTICE " + channelName + " :" + msg);
    }
}

void	Server::notice(int fd, std::vector<std::string> token) {
    if (token[token.size() - 1][0] != ':' || token.size() != 3) {
        ft_write(fd, ":" + _clients[fd]->getPrefixName() + " 461 " + _clients[fd]->getNickName() + " NOTICE :Not enough parameters\r\n");
        return ;
    }

    int nickFd = findUserByName(token[1]);
    if (_clients.find(nickFd) != _clients.end()) { // FOUND USER
        ft_write(_clients[nickFd]->getFd(), ":" + _clients[fd]->getPrefixName() + " NOTICE " + token[1] + " :" + token[2]);
        return ;
    }
    else if (_channels.find(token[1]) != _channels.end()) { // FOUND CHANNEL
        if (_channels[token[1]]->getClientAuthority())
		{
			size_t	i = 0;
			for (i = 0 ; i < _channels[token[1]]->_channelClients.size() ; i++)
				if (_channels[token[1]]->_channelClients[i]->getNickName() == _clients[fd]->getNickName())
					break;
			if (i == _channels[token[1]]->_channelClients.size())
			{
				_clients[fd]->clientMsgSender(fd, ERR_CANNOTSENDTOCHAN(_clients[fd]->getNickName(), token[1]));
				return;
			}
		}
        broadcastNotice(_channels.find(token[1])->second->_channelClients, token[2], fd, token[1]);
        return ;
    }
    ft_write(fd, ":" + _clients[fd]->getPrefixName() + " 401 " + token[1] + ":No such Nick/Channel\r\n"); //NO USER OR CHANNEL MATCHES
}

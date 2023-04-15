#include "../../headers/Server.hpp"

void    Server::broadcastPart(const std::vector<Client *> &clientList, std::string msg, int excludeFd, std::string channelName) {
    for (size_t i = 0; i < clientList.size(); i++)
    {
        if (clientList[i]->getFd() == excludeFd)
            continue ;
        ft_write(clientList[i]->getFd(), ":" + _clients[excludeFd]->getPrefixName() + " PART " + channelName + " :" + msg);
    }
}

void	Server::part(int fd, std::vector<std::string> token) {
    std::string msg;

    if (token.empty() or token.size() < 2) {
		_clients[fd]->clientMsgSender(fd, ERR_NEEDMOREPARAMS(_clients[fd]->getNickName(), "PART"));
		return;
	}

	std::string name = token[1];
	Channel *channel = _channels[name];
	if (!channel) {
		_clients[fd]->clientMsgSender(fd, ERR_NOSUCHCHANNEL(_clients[fd]->getNickName(), "PART"));
		return;
	}

    size_t i;
	for (i = 0 ; i < _clients[fd]->_channels.size() ; i++)
		if (_clients[fd]->_channels[i]->getName() == name)
            break;

    if (i == _clients[fd]->_channels.size())
    {
		_clients[fd]->clientMsgSender(fd, ERR_NOTONCHANNEL(_clients[fd]->getNickName(), "PART"));
        return;
    }

    broadcastPart(channel->_channelClients , RPL_PART(_clients[fd]->getPrefixName(), channel->getName()), fd, name);
	msg = ":" + _clients[fd]->getPrefixName() + " PART " + name;
	ft_write(fd, msg);
	_clients[fd]->_channels[i]->leftTheChannel(_clients[fd]);
    _clients[fd]->_channels.erase(_clients[fd]->_channels.begin() + i);
    if (_clients[fd]->_channels[i]->getClientCount() == 0)
	{
		std::cout << _clients[fd]->_channels[i]->getName() << " Channel delete...\n";
		_channels.erase(_clients[fd]->_channels[i]->getName());
	}
}

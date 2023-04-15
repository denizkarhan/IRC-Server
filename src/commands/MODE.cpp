#include "../../headers/Server.hpp"

void broadcasting(const std::string &message, std::vector<Client *> _clients) {
	for (size_t i = 0 ; i < _clients.size() ; i++)
		ft_write(_clients[i]->getFd(), message);
}

void	Server::mode(int fd, std::vector<std::string> token) {
    token.erase(token.begin());
	if (token.size() < 2 || token[1].size() < 2) {
		_clients[fd]->clientMsgSender(fd, ERR_NEEDMOREPARAMS(_clients[fd]->getNickName(), "MODE"));
		return;
	}

	std::string target = token.at(0);

	Channel *channel = _channels[target];
	if (!channel) {
		_clients[fd]->clientMsgSender(fd, ERR_NOSUCHCHANNEL(_clients[fd]->getNickName(), target));
		return;
	}

	if (channel->getAdmin() != _clients[fd]) {
		_clients[fd]->clientMsgSender(fd, ERR_CHANOPRIVSNEEDED(_clients[fd]->getNickName(), target));
		return;
	}

	bool	i = (token[1][0] == '+');
	char	mod = token[1][1];

	if (mod == 'k') {
			channel->setPassword((i && token.size() > 2) ? token[2] : "");
			broadcasting(RPL_MODE(_clients[fd]->getPrefixName(), channel->getName(), (i ? "+k" : "-k"), (i ? token[2] : "")), _channels[target]->_channelClients);
	}
	else if (mod == 'l') {
			channel->setMaxClientCount((i && token.size() > 2) ? std::atoi(token[2].c_str()) : 1000000);
			broadcasting(RPL_MODE(_clients[fd]->getPrefixName(), channel->getName(), (i ? "+l" : "-l"), (i ? token[2] : "")), _channels[target]->_channelClients);
	}
	else if (mod == 'n') {
			channel->setClientAuthority(i);
			broadcasting(RPL_MODE(_clients[fd]->getPrefixName(), channel->getName(), (i ? "+n" : "-n"), ""), _channels[target]->_channelClients);
	}
}

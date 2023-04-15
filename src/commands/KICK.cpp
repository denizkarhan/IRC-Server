#include "../../headers/Server.hpp"

void	Server::kick(int fd, std::vector<std::string> token) {
	std::string	msg;

	if (token.size() < 3) {
		_clients[fd]->clientMsgSender(fd, ERR_NEEDMOREPARAMS(_clients[fd]->getNickName(), "KICK"));
		return;
	}

	std::string channelName = token[1].substr(token[1][0] == ':', token[1].size());
	std::string target = token[2].substr(token[2][0] == ':', token[2].find(' '));
	if (target[target.size() - 1] == ' ')
		target = target.substr(0, target.size() - 1);

	size_t	channelFinder = 0;
	for (channelFinder = 0 ; channelFinder < _clients[fd]->_channels.size() ; channelFinder++)
		if (_clients[fd]->_channels[channelFinder]->getName() == channelName)
			break;

	if (_clients[fd]->_channels.size() == 0 || channelFinder == _clients[fd]->_channels.size()) {
		_clients[fd]->clientMsgSender(fd, ERR_NOSUCHCHANNEL(_clients[fd]->getNickName(), "KICK"));
		return ;
	}

	if (_clients[fd]->_channels[channelFinder]->getAdmin() != _clients[fd]) {
		_clients[fd]->clientMsgSender(fd, ERR_CHANOPRIVSNEEDED(_clients[fd]->getNickName(), "KICK"));
		return ;
	}

	int	targetClientFd = findUserByName(target);
	if (targetClientFd == -1) {
		_clients[fd]->clientMsgSender(fd, ERR_USERNOTINCHANNEL(_clients[fd]->getNickName(), "KICK", channelName));
		return;
	}

	int	removeChannelIndex = -1;
	for (size_t i = 0 ; i < _clients[targetClientFd]->_channels.size() ; i++)
		if (_clients[targetClientFd]->_channels[i]->getName() == channelName)
			removeChannelIndex = i;

	if (removeChannelIndex == -1) {
		_clients[fd]->clientMsgSender(fd, ERR_NOTONCHANNEL(_clients[fd]->getNickName(), "KICK"));
		return;
	}

	std::cout << std::endl << "Channel's new admin is:	" << _channels[channelName]->getAdmin()->getNickName() << std::endl;
	for (size_t i = 0 ; i < _channels[channelName]->_channelClients.size() ; i++)
		std::cout << i + 1 << "->	"<< _channels[channelName]->_channelClients[i]->getNickName() << std::endl;
	
	broadcast(_channels[channelName]->_channelClients, RPL_KICK(_clients[fd]->getNickName(), channelName, target, token[2]), fd);
	ft_write(fd, RPL_KICK(_clients[fd]->getNickName(), channelName, target, token[2]));
	_channels[channelName]->leftTheChannel(_clients[targetClientFd]);
	_clients[targetClientFd]->_channels.erase(_clients[targetClientFd]->_channels.begin() + removeChannelIndex);
	if (_channels[channelName]->getClientCount() == 0)
	{
		std::cout << channelName << " Deleting the channel...\n";
		_channels.erase(channelName);
	}

	if (_channels.find(channelName) != _channels.end() && _channels[channelName]->getClientCount() > 0)
	{
		std::cout << std::endl << "Channel's new admin is:	" << _channels[channelName]->getAdmin()->getNickName() << std::endl;
		for (size_t i = 0 ; i < _channels[channelName]->_channelClients.size() ; i++)
			std::cout << i + 1 << "->	" << _channels[channelName]->_channelClients[i]->getNickName() << std::endl;
	}
}

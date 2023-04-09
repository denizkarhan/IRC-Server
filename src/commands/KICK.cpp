#include "../../headers/Server.hpp"

void	Server::kick(int fd, std::vector<std::string> token)
{
	std::string	msg;
	if (token.size() < 3) {
		msg = "Eksik parametre girdiniz\r\n";
		send(fd, msg.c_str(), msg.size(), 0);
		return;
	}

	std::string name = token[2].substr(token[2][0] == ':', token[2].size());
	std::string target = token[3].substr(token[3][0] == ':', token[3].size());
	std::string reason = "";

	if (token.size() > 4 && (token[4][0] != ':' || token[4].size() > 1)) {
		reason = "";
		for (int it = 4; it < token.size() ; it++)
			reason.append(token[it] + " ");
	}

	int	channelFinder = 0;
	for (channelFinder = 0 ; channelFinder < _clients[fd]->_channels.size() ; channelFinder++)
		if (_clients[fd]->_channels[channelFinder]->getName() == name)
			break;

	if (_clients[fd]->_channels.size() == 0 || channelFinder == _clients[fd]->_channels.size()) {
		msg = "Kanal bulunamadı!\r\n";
		send(fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	if (_clients[fd]->_channels[channelFinder]->getAdmin() != _clients[fd]) {
		msg = "Admin yetkiniz yoktur!\r\n";
		send(fd, msg.c_str(), msg.size(), 0);
		return;
	}

	int	targetClientFd = -1;
	for (int i = 0 ; i < _clients[fd]->_channels[channelFinder]->_channelClients.size() ; i++)
		if (_clients[fd]->_channels[channelFinder]->_channelClients[i]->getNickName() == target)
			targetClientFd = _clients[fd]->_channels[channelFinder]->_channelClients[i]->getFd();

	if (targetClientFd == -1) {
		msg = "Aranan nickname ile kullanıcı bulunamadı!\r\n";
		send(fd, msg.c_str(), msg.size(), 0);
		return;
	}

	int	removeChannelIndex = -1;
	for (int i = 0 ; i < _clients[targetClientFd]->_channels.size() ; i++)
		if (_clients[targetClientFd]->_channels[i]->getName() == name)
			removeChannelIndex = i;

	if (removeChannelIndex == -1) {
		msg = "Kullanıcı kanala üye değil!\r\n";
		send(fd, msg.c_str(), msg.size(), 0);
		return;
	}

	std::cout << std::endl << "Kanalın eski admini:	" << _channels[name]->getAdmin()->getNickName() << std::endl;
	for (int i = 0 ; i < _channels[name]->_channelClients.size() ; i++)
		std::cout << i + 1 << "->	"<< _channels[name]->_channelClients[i]->getNickName() << std::endl;
	
	_channels[name]->leftTheChannel(_clients[targetClientFd]);
	if (_channels[name]->getClientCount() == 0)
	{
		std::cout << name << " Kanalı Siliniyor...\n";
		_channels.erase(_channels.find(_channels[name]->getName()));
	}

	if (_channels.find(name) != _channels.end() && _channels[name]->getClientCount() > 0)
	{
		std::cout << std::endl << "Kanalın yeni admini:	" << _channels[name]->getAdmin()->getNickName() << std::endl;
		for (int i = 0 ; i < _channels[name]->_channelClients.size() ; i++)
			std::cout << i + 1 << "->	" << _channels[name]->_channelClients[i]->getNickName() << std::endl;
	}
	
	msg = target + " Kanaldan ayrıldı. " + reason + "\r\n";
	send(fd, msg.c_str(), msg.size(), 0);
	serverInfo(msg);
}

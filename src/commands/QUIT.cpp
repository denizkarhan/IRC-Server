#include "../../headers/Server.hpp"

void	Server::quit(int fd, std::vector<std::string> token)
{
	for (size_t i = 0 ; i < _pollfds.size() ; i++)
	{
		if (fd == _pollfds[i].fd)
		{
			std::string msg = ":" + _clients[fd]->getPrefixName() + " QUIT :Quit " + token[token.size() - 1];
			ft_write(fd, msg);
			close(_pollfds[i].fd);
			_pollfds.erase(_pollfds.begin() + i);
		}
	}
	for (size_t i = 0 ; i < _clients[fd]->_channels.size() ; i++) // client serverdan ayrılırken bulunduğu tüm kanallardan ayrılmalı.
	{
		_clients[fd]->_channels[i]->leftTheChannel(_clients[fd]);
		if (_clients[fd]->_channels[i]->getClientCount() == 0)
		{
			std::cout << "Channel delete...\n";
			_channels.erase(_channels.find(_clients[fd]->_channels[i]->getName()));
		}
	}
	_clients.erase(fd); // server'dan kullanıcıyı sildim.
	_usrCount--;
}

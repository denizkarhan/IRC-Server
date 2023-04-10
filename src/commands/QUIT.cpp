#include "../../headers/Server.hpp"

void	Server::quit(int fd, std::vector<std::string> token) { // send komutları quit fonksiyonuna gelir gelmez kullanıcının fd'si kapandığı için hata veriyor, neden kapanıyor?
	std::cout << "QUIT Function started\n";

	(void) token;
	Client *clientPtr = _clients.at(fd);
	std::string msg;

	// Kullanıcı /quit ile çıkarken argüman olarak ayrılık mesajı da verebilir Verdiği argümanları ' ' ile birleştirip client'e göndermeliyiz.
	// msg = _clients[fd]->getNickName() + " successfully quitted\r\n"; send(fd, msg.c_str(), msg.size(), 0);

	for (size_t i = 0 ; i < _pollfds.size() ; i++)
	{
		if (fd == _pollfds[i].fd)
		{
			close(_pollfds[i].fd);
			_pollfds.erase(_pollfds.begin() + i);
		}
	}
	for (size_t i = 0 ; i < _clients[fd]->_channels.size() ; i++) // client serverdan ayrılırken bulunduğu tüm kanallardan ayrılmalı.
	{
		msg = _clients[fd]->getNickName() + " left the " + _clients[fd]->_channels[i]->getName() + "\r\n";
		std::cout << msg;
		_clients[fd]->_channels[i]->leftTheChannel(_clients[fd]);
		if (_clients[fd]->_channels[i]->getClientCount() == 0)
		{
			std::cout << "Kanal Siliniyor...\n";
			_channels.erase(_channels.find(_clients[fd]->_channels[i]->getName()));
		}
		// send(fd, msg.c_str(), msg.size(), 0);
	}
	_clients.erase(fd); // server'dan kullanıcıyı sildim.
	delete clientPtr;
	_usrCount--;
}

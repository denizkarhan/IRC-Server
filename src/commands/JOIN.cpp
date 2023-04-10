#include "../../headers/Server.hpp"

void	Server::join(int fd, std::vector<std::string> token) {
	// std::cout << "JOIN Function started.\n";

	std::string	msg;
	if (token.empty() || token[1].empty()) {
		msg = "JOIN komutunda yeterli argüman yok!\r\n";
		send(fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	if (token[1][0] != '#')
		token[1] = "#" + token[1];
	
	if (token[1].size() < 2) // "/join #" kontrolü
	{
		msg = "Kanal belirtmediniz!\r\n";
		send(fd, msg.c_str(), msg.size(), 0);
		return ;
	}
	
	if (_channels.find(token[1]) == _channels.end())
	{
		// Admin girişi, kanal oluşturuluyor.
		msg = token[1] + " kanalı oluşturuluyor\r\n"; send(fd, msg.c_str(), msg.size(), 0);
		msg = "Admin girişi\r\n"; send(fd, msg.c_str(), msg.size(), 0);
		_channels[token[1]] = new Channel(_clients[fd], token[1], token.size() > 2 ? token[2] : "");
		_clients[fd]->_channels.push_back(_channels[token[1]]);

		_clients[fd]->clientMsgSender(fd, RPL_NAMREPLY(_clients[fd]->getNickName(), token[1], _channels[token[1]]->getUsers()));
		_clients[fd]->clientMsgSender(fd, RPL_ENDOFNAMES(_clients[fd]->getNickName(), token[1]));

		_clients[fd]->casting(fd, _channels[token[1]]->_channelClients, RPL_JOIN(_clients[fd]->getPrefixName(), token[1]));

		return ;
	}

	for (int i = 0 ; i < _clients[fd]->_channels.size() ; i++)
	{
		if (_clients[fd]->_channels[i]->getName() == token[1])
		{
			msg = "Zaten bu kanaldasınız!\r\n";
			send(fd, msg.c_str(), msg.size(), 0);
			return ;
		}
	}
	if (_channels[token[1]]->getClientCount() >= _channels[token[1]]->getMaxClientCount())
		msg = "Kanaldaki kullanıcı sayısı doldu!\r\n";
	else if (!(_channels[token[1]]->getPassword().empty()) && _channels[token[1]]->getPassword() != token[2])
		msg = "Kanala girilemedi, parola hatası!\r\n";
	else
	{
		// Üye girişi
		msg = "Üye girişi\r\n";
		_channels[token[1]]->_channelClients.push_back(_clients[fd]);
		_clients[fd]->_channels.push_back(_channels[token[1]]);
		_channels[token[1]]->setClientCount(_channels[token[1]]->getClientCount() + 1);
	}
	std::cout << "Kullanıcı sayısı:	" << _channels[token[1]]->getClientCount() << std::endl;
	send(fd, msg.c_str(), msg.size(), 0);
}

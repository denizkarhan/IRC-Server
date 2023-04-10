#include "../../headers/Server.hpp"

void	Server::part(int fd, std::vector<std::string> token) {
    std::string msg;

    if (token.empty() or token.size() < 2) {
		msg = "Daha fazla parametre gerekli!\r\n";
        send(fd, msg.c_str(), msg.size(), 0);
		return;
	}

	std::string name = token[1];
	Channel *channel = _channels[name];
	if (!channel) {
		msg = "Kanal bulunamadı!\r\n";
        send(fd, msg.c_str(), msg.size(), 0);
		return;
	}

    size_t i;
	for (i = 0 ; i < _clients[fd]->_channels.size() ; i++)
		if (_clients[fd]->_channels[i]->getName() == name)
            break;

    if (i == _clients[fd]->_channels.size())
    {
        msg = "Kullanıcı kanalda değil!\r\n";
        send(fd, msg.c_str(), msg.size(), 0);
        return;
    }

    _clients[fd]->_channels[i]->leftTheChannel(_clients[fd]);
}

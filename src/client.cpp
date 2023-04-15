#include "../headers/Client.hpp"

Client::Client(int fd, int port)  {
	_status = 0;
    _fd = fd;
    _port = port;
}

Client::~Client() {}

void Client::leave() {
	if (!_channels.size())
		return ;

    for (size_t i = 0 ; i < _channels.size() ; i+= 1)
    {
    	const std::string name = _channels[i]->getName();

    	_channels[i]->leftTheChannel(this);
    	char message[100];
    	sprintf(message, "%s has left channel %s.", _nickname.c_str(), _username.c_str());
    	serverInfo(message);
    }
}

std::string	Client::getPrefixName() {
    std::string prefixName = _nickname;
    if (!(_username.empty()))
        prefixName += '!' + _username;
    if (!(_hostname.empty()))
        prefixName += '@' + _hostname;
    return (prefixName);
}

void	Client::clientMsgSender(int _fd, std::string const &msg) {
    ft_write(_fd, ":" + getPrefixName() + " " + msg);
}

void	Client::casting(int _fd, std::vector<Client *> _clients, const std::string &message) {
    for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
        ft_write(_fd, message);
}
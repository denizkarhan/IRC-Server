#include "../headers/Client.hpp"

Client::Client(int fd, int port) 
{
	_status = 0;
    _fd = fd;
    _port = port;
}

Client::~Client() {}

void Client::leave()
{
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

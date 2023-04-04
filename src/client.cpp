#include "../headers/Client.hpp"

Client::Client(int fd, int port, std::string const &host) 
{
    _fd = fd;
    _port = port;
    _hostname = host;
}

Client::~Client() {}
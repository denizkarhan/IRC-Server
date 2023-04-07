#include "../headers/Client.hpp"

Client::Client(int fd, int port) 
{
    _fd = fd;
    _port = port;
}

Client::~Client() {}


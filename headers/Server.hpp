#ifndef SERVER_HPP
# define SERVER_HPP

#include "Utils.hpp"
#include "Client.hpp"

class Server
{
    private:
        int _socketFd;
        int _port;
        std::string _password;

        std::vector<Client *> _clients;
        std::vector<Channel *>  _channels;

    public:
        Server(char **av);
        ~Server();

        int createSocket();
};

#endif
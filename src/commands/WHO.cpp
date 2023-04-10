#include "../../headers/Server.hpp"

void	Server::who(int fd, std::vector<std::string> token)
{
    if (token.empty() || token.size() < 2)
        return;

    std::string msg = "Kanaldakiler\n";
    for (size_t i = 0 ; i < _channels[token[1]]->_channelClients.size() ; i++)
        msg += _channels[token[1]]->_channelClients[i]->getNickName() + "\n";
    msg += "\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}
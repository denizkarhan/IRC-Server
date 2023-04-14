#include "../../headers/Server.hpp"

void	Server::who(int fd, std::vector<std::string> token)
{
    if (token.empty() || token.size() < 2)
        return;

    if (_channels.find(token[1]) != _channels.end())
    {    
        for (size_t i = 0 ; i < _channels[token[1]]->_channelClients.size() ; i++)
        {    
            std::string msg = ":" + _clients[fd]->getPrefixName() + " 352 " + _clients[fd]->getNickName() + " " + token[1];
            msg += " " + _channels[token[1]]->_channelClients[i]->getUserName();
            msg += " " + _channels[token[1]]->_channelClients[i]->getUserName();
            msg += " " + _channels[token[1]]->_channelClients[i]->getPrefixName();
            msg += " " + _channels[token[1]]->_channelClients[i]->getNickName();
            msg += " H";
            msg += ":0 " + _channels[token[1]]->_channelClients[i]->getRealName();
            ft_write(fd, msg);
        }
    }
}
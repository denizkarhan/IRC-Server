#include "../../headers/Server.hpp"
/**
 * @brief 
 * 
 * @param name name of the user
 * @return int returns the fd value of the said user or returns -1 if none found
 */
int Server::findUserByName(std::string name)
{
    for (std::map<int, Client *>::iterator it = _clients.begin() ; it != _clients.end() ; it++)
        if (it->second->getNickName() == name)
            return it->first;
    return -1;
}

void    Server::broadcast(const std::vector<Client *> &clientList, std::string msg, int excludeFd)
{
    for (size_t i = 0; i < clientList.size(); i++)
    {
        if (clientList[i]->getFd() == excludeFd)
            continue ;
        ft_write(clientList[i]->getFd(), msg);
    }
}

void    Server::privmsg(int fd, std::vector<std::string> token)
{
	if (token.size() < 3) {
		_clients[fd]->clientMsgSender(fd, ERR_NEEDMOREPARAMS(_clients[fd]->getNickName(), "PRIVMSG"));
		return;
	}

	if (token[2][0] == ':')
		token[2].erase(0, 1);

	if (token[1][0] == '#') {
		Channel *channel = _channels[token[1]];
		if (!channel)
		{
			_clients[fd]->clientMsgSender(fd, ERR_NOSUCHCHANNEL(_clients[fd]->getNickName(), token[1]));
			return;
		}

		if (channel->getClientAuthority())
		{
			size_t	i = 0;
			for (i = 0 ; i < _channels[token[1]]->_channelClients.size() ; i++) // Kanala mesaj yollamak için şimdiki kullanıcının kanalda olması gereklidir.
				if (_channels[token[1]]->_channelClients[i]->getNickName() == _clients[fd]->getNickName())
					break;
			if (i == _channels[token[1]]->_channelClients.size())
			{
				_clients[fd]->clientMsgSender(fd, ERR_CANNOTSENDTOCHAN(_clients[fd]->getNickName(), token[1]));
				return;
			}
		}
		broadcast(channel->_channelClients, RPL_PRIVMSG(_clients[fd]->getPrefixName(), token[1], token[2]), fd);
		return;
	}

	int destFd = findUserByName(token[1]);
	if (destFd == -1)
	{
		_clients[fd]->clientMsgSender(fd, ERR_NOSUCHNICK(_clients[fd]->getNickName(), token[1]));
		return;
	}
	ft_write(destFd, RPL_PRIVMSG(_clients[fd]->getPrefixName(), token[1], token[2]));
}

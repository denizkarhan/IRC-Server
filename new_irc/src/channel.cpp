#include "../headers/Channel.hpp"

Channel::Channel(Client *admin, std::string const &name, std::string const &password)
{
    _channelAdmin = admin;
    _channelName = name;
    _channelPassword = password;
    _clientAuthority = true;
    _ClientCount = 0;
    _maxClientCount = 100;
}

Channel::~Channel()
{

}

void    Channel::leftTheChannel(Client *client)
{
    for (int i = 0 ; i < _channelClients.size() ; i++)
    {
        if (_channelClients[i] == client)
        {
            _channelClients.erase(_channelClients.begin() + i);
            break;
        }
    }
}

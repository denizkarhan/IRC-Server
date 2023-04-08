#include "../headers/Channel.hpp"

Channel::Channel(Client *admin, std::string const &name, std::string const &password)
{
    _channelAdmin = admin;
    _channelClients.push_back(admin);
    _ClientCount = 1;
    _channelName = name;
    _channelPassword = password;
    _clientAuthority = true;
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
            if (_channelAdmin == client && _ClientCount > 1)
            {
                std::cout << _channelAdmin->getNickName() << " kanaldan ayrıldı yeni admin " << _channelClients[1]->getNickName() << std::endl;
                _channelAdmin = _channelClients[1];
            }
            _channelClients.erase(_channelClients.begin() + i);
            _ClientCount--;
            break;
        }
    }
}

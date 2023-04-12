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

    _k = password;
    _l = 0;
    _n = false;
}

Channel::~Channel() {}

void    Channel::leftTheChannel(Client *client)
{
    for (size_t i = 0 ; i < _channelClients.size() ; i++)
    {
        if (_channelClients[i] == client)
        {
            if (_channelAdmin == client)
            {
                std::string msg = "Channel's admin " + _channelAdmin->getNickName() + " quit the channel, ";
                if (_ClientCount > 1)
                {
                    msg = msg + "yeni admin " + _channelClients[1]->getNickName() + "\r\n";
                    _channelAdmin = _channelClients[1];
                    setAdmin(_channelClients[1]);
                }
                else
                {
                    msg = msg + getName() + " kanal siliniyor.\r\n";
                    setAdmin(NULL);
                }
                std::cout << msg << std::endl;
                send(client->getFd(), msg.c_str(), msg.size(), 0);
            }
            _channelClients.erase(_channelClients.begin() + i);
            _ClientCount--;
            break;
        }
    }
}

std::string     Channel::getUsers()
{
    std::string users;
    for (size_t i = 0 ; i < _channelClients.size() ; i++)
	{
        users += _channelClients[i]->getNickName();
        if (i + 1 < _channelClients.size())
            users += ",";
    }
    return users;
}

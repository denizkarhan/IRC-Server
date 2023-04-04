#include "../headers/Channel.hpp"

Channel::Channel(Client *admin, std::string const &name, std::string const &password)
{
    _channelAdmin = admin;
    _channelName = name;
    _channelPassword = password;
    _clientAuthority = true;
    _maxClientCount = 100;
}

Channel::~Channel()
{

}


#ifndef CHANNEL_HPP
# define CHANNEL_HPP

class Channel;

#include "Client.hpp"

class Channel
{
    private:
        int     _maxClientCount;
        bool    _clientAuthority;
        std::string  _channelName;
        std::string  _channelPassword;

        Client  *_channelAdmin;
        std::vector<Client *>  _channelClients;

    public:
        Channel(Client *admin, std::string const &name, std::string const &password);
        ~Channel();

        Client			*getAdmin() { return _channelAdmin; }
        std::string		getName() { return _channelName; }
        std::string		getPassword() { return _channelPassword; }
        int				getClientCount() { return _maxClientCount; }
        bool			getClientAuthority() { return _clientAuthority; }

        void    setAdmin(Client *newAdmin) { _channelAdmin = newAdmin; }
        void    setName(std::string const &newName) { _channelName = newName; }
        void    setPassword(std::string const &newPassword) { _channelPassword = newPassword; }
        void    setClientCount(int newClientCount) { _maxClientCount = newClientCount; }
        void    setClientAuthority(bool auth) { _clientAuthority = auth; }
};

#endif

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

class Channel;

#include "Client.hpp"

class Channel
{
    private:
        int             _ClientCount;
        std::string     _channelName;
		std::string				_k;
		size_t					_l;
		bool					_n;

        Client          *_channelAdmin;

    public:
        Channel(Client *admin, std::string const &name, std::string const &password);
        ~Channel();

        std::vector<Client *>   _channelClients;

        Client			        *getAdmin() { return _channelAdmin; }
        std::string		        getName() { return _channelName; }
        int				        getClientCount() { return _ClientCount; }
        std::string		        getPassword() { return _k; }
        int				        getMaxClientCount() { return _l; }
        bool			        getClientAuthority() { return _n; }

        void                    setAdmin(Client *newAdmin) { _channelAdmin = newAdmin; }
        void                    setName(std::string const &newName) { _channelName = newName; }
        void                    setClientCount(int newClientCount) { _ClientCount = newClientCount; }
        void                    setPassword(std::string const &k) { _k = k; }
        void                    setMaxClientCount(int l) { _l = l; }
        void                    setClientAuthority(bool n) { _n = n; }

        void                    leftTheChannel(Client *client);
        std::string             getUsers();

};

#endif

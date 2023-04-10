#ifndef CHANNEL_HPP
# define CHANNEL_HPP

class Channel;

#include "Client.hpp"

class Channel
{
    private:
        int             _maxClientCount;
        int             _ClientCount;
        bool            _clientAuthority;
        std::string     _channelName;
        std::string     _channelPassword;

        /* Modes */
		std::string				_k; // set a channel key password
		size_t					_l; // set the user limit to channel, l = 0
		bool					_n; // no messages to channel from clients on the outside

        Client          *_channelAdmin;

    public:
        Channel(Client *admin, std::string const &name, std::string const &password);
        ~Channel();

        std::vector<Client *>   _channelClients;

        Client			        *getAdmin() { return _channelAdmin; }
        std::string		        getName() { return _channelName; }
        std::string		        getPassword() { return _channelPassword; }
        int				        getMaxClientCount() { return _maxClientCount; }
        int				        getClientCount() { return _ClientCount; }
        bool			        getClientAuthority() { return _clientAuthority; }
        std::string			    getK() { return _k; }
        int			            getL() { return _l; }
        bool			        getN() { return _n; }

        void                    setAdmin(Client *newAdmin) { _channelAdmin = newAdmin; }
        void                    setName(std::string const &newName) { _channelName = newName; }
        void                    setPassword(std::string const &newPassword) { _channelPassword = newPassword; }
        void                    setMaxClientCount(int newMaxClientCount) { _maxClientCount = newMaxClientCount; }
        void                    setClientCount(int newClientCount) { _ClientCount = newClientCount; }
        void                    setClientAuthority(bool auth) { _clientAuthority = auth; }
        void                    setK(std::string k) { _k = k; }
        void                    setL(int l) { _l = l; }
        void                    setN(bool n) { _n = n; }

        void                    leftTheChannel(Client *client);
        std::string             getUsers();

};

#endif

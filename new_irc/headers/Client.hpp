#ifndef CLIENT_HPP
# define CLIENT_HPP

class Client;

#include "Utils.hpp"
#include "Channel.hpp"

class Client
{
    private:
        int _fd;
        int _port;

        std::string _username;
        std::string _nickname;
        std::string _realname;
        std::string _hostname;

        Channel     *_channels;

    public:
        Client(int fd, int port, std::string const &host);
        ~Client();

        void    setUserName(std::string const &username) { _username = username; }
        void    setNickName(std::string const &nickname) { _nickname = nickname; }
        void    setRealName(std::string const &realname) { _realname = realname; }
        void    setHostName(std::string const &hostname) { _hostname = hostname; }

        std::string getUserName() const { return _username; }
        std::string getNickName() const { return _nickname; }
        std::string getRealName() const { return _realname; }
        std::string getHostName() const { return _hostname; }

        int getFd() const { return _fd; }
        int getPort() const { return _port; }
};

#endif

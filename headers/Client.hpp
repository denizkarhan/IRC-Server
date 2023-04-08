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
        int _status;

        std::string     _username;
        std::string     _nickname;
        std::string     _password;

    public:
        Client(int fd, int port);
        ~Client();

        std::vector<Channel *>    _channels;

        std::string getUserName() const { return _username; }
        std::string getNickName() const { return _nickname; }
        std::string getpassword() const { return _password; }
        int         getStatus() const { return _status; }

        void    setUserName(std::string const &username) { _username = username; }
        void    setNickName(std::string const &nickname) { _nickname = nickname; }
        void    setPassword(std::string const &password) { _password = password; }
        void    setStatus(int status) { _status = status; }

        int getFd() const { return _fd; }
        int getPort() const { return _port; }

		void leave();

};

#endif

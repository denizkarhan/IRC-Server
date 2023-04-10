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
        std::string     _realname;
        std::string     _hostname;
        std::string     _password;

    public:
        Client(int fd, int port);
        ~Client();

        std::vector<Channel *>    _channels;

        int         getStatus() const { return _status; }
        std::string getUserName() const { return _username; }
        std::string getNickName() const { return _nickname; }
        std::string getRealName() const { return _realname; }
        std::string getHostName() const { return _hostname; }
        std::string getpassword() const { return _password; }

        void    setStatus(int status) { _status = status; }
        void    setUserName(std::string const &username) { _username = username; }
        void    setNickName(std::string const &nickname) { _nickname = nickname; }
        void    setRealName(std::string const &realname) { _realname = realname; }
        void    setHostName(std::string const &hostname) { _hostname = hostname; }
        void    setPassword(std::string const &password) { _password = password; }

        int getFd() const { return _fd; }
        int getPort() const { return _port; }

		void leave();
        std::string getPrefixName()
        {
            std::string prefixName = _nickname;

            if (!(_username.empty()))
                prefixName += '!' + _username;
            if (!(_hostname.empty()))
                prefixName += '@' + _hostname;
            return (prefixName);
        }

        void clientMsgSender(int _fd, std::string const &msg)
        {
            ft_write(_fd , ":" + getPrefixName() + " " + msg);
        }

        void casting(int _fd, std::vector<Client *> _clients, const std::string &message)
        {
            for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
                ft_write(_fd, message);
        }
};

#endif

#include "Utils.hpp"
#include "Client.hpp"

class Server {
    private:
        int                         _port;
        int                         _socketFd;
        int                         _usrCount;
        char                        _buffer[BUFFER_SIZE];
        char*                       _password;
        struct sockaddr_in          _addr;
        
        std::vector<pollfd>		            _pollfds;
        std::map<int, Client *>             _clients;
        std::map<std::string, Channel *>    _channels;
        std::map<std::string, void(Server::*)(int, std::vector<std::string>)> _commands;

    public:
        Server(char **av);
        ~Server();
        
        int                 getPort() { return _port; }
        int                 getServerfd() { return _socketFd; }
        char*               getPassword() { return _password; }
        struct sockaddr_in  getAddr() { return _addr; }
        
        void                setPort(int port) { _port = port; }
        void                setServerfd(int server_fd) { _socketFd = server_fd; }
        void                setPassword(char* password) { _password = password; }
        void                setAddr(struct sockaddr_in addr) { _addr = addr; }

        int                 createSocket();
        void                loop();
        void                readMessage(int fd);
        void                newConnection();
        void                serverInfo(std::string message);
        
        void                welcome(int fd, std::vector<std::string> token);
        void                quit(int fd, std::vector<std::string> token);
        void                join(int fd, std::vector<std::string> token);
        void                pass(int fd, std::vector<std::string> token);
        void                nick(int fd, std::vector<std::string> token);
        void                user(int fd, std::vector<std::string> token);
        void                kick(int fd, std::vector<std::string> token);
        void                part(int fd, std::vector<std::string> token);
        void                ping(int fd, std::vector<std::string> token);
        void                pong(int fd, std::vector<std::string> token);
        void                notice(int fd, std::vector<std::string> token);

};

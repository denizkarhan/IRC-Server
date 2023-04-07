#include "Utils.hpp"
#include "Client.hpp"

class Server{
    private:
        struct sockaddr_in  _addr;
        int                 _socketFd;
        int                 _port;
        char*               _password;
        int                 _usrCount;
        int                 _addrLen;
        char                _buffer[BUFFER_SIZE];
        struct pollfd       _pollFds[USER_MAX];

        std::map<int, Client *>             _clients;
        std::map<std::string, Channel *>    _channels;
        std::map<std::string, void(Server::*)(int, std::vector<std::string>)> _map;

    public:
        Server(char **av);
        ~Server();
        int                 getPort();
        int                 getServerfd();
        char*               getPassword();
        int                 getAddrlen();
        struct sockaddr_in  getAddr();
        

        void                setPort(int port);
        void                setServerfd(int server_fd);
        void                setPassword(char* password);
        void                setAddrlen(int addrLen);
        void                setAddr(struct sockaddr_in addr);
        

        int                 createSocket();
        void                serverInfo(std::string message);


        void                loop();
        void                readMessage(int fd);
        void                newUser(int fd, std::vector<std::string> tokens);

        void                quit(int fd, std::vector<std::string> token);
        void                join(int fd, std::vector<std::string> token);
        void                welcome(int fd, std::vector<std::string> token);
        void                pass(int fd, std::vector<std::string> token);
        void                nick(int fd, std::vector<std::string> token);
        void                user(int fd, std::vector<std::string> token);
};
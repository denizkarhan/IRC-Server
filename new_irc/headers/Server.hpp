#include "Utils.hpp"
#include "Client.hpp"

class Server{
    private:
        struct sockaddr_in  _addr;
        socklen_t           _addr_len;
        int                 _server_fd;
        int                 _port;
        char*               _password;
        char                _buffer[BUFFER_SIZE];
        struct pollfd       _pollfds[USER_SIZE];

        std::map<int, Client *> _clients;

    public:
        Server(char **av);
        ~Server();
        void                createSocket();
        int                 getPort();
        int                 getServerfd();
        char*               getPassword();
        socklen_t           getAddrlen();
        struct sockaddr_in  getAddr();
        
        void                setPort(int port);
        void                setServerfd(int server_fd);
        void                setPassword(char* password);
        void                setAddrlen(socklen_t addrLen);
        void                setAddr(struct sockaddr_in addr);
        
        void                createSocket();
        void                serverInfo(char *message);
};
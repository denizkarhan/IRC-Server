#include "../headers/Server.hpp"

//Constructor

Server::Server(char **av)
{
    _port = errCheck(0, atoi(av[1]), "Not a usable port!");
    _password = av[2];
    _addr_len = sizeof(_addr);
    std::memset(&_addr, 0, sizeof(_addr));
    _addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = INADDR_ANY;
	_addr.sin_port = htons(_port);
    createSocket();
}

//Destructor

Server::~Server(){}

//Methods

void Server::createSocket()
{
	int (temp) = 1;

	_pollfds[0].fd = _server_fd;
	_pollfds[0].events = POLLIN;
	for (int i = 1; i < USER_SIZE; i++){
		_pollfds[i].fd = -1;
		_pollfds[i].events = POLLIN;
	}
    _server_fd = errCheck(-1, socket(AF_INET, SOCK_STREAM, 0), "Error: failed to create socket.");
	errCheck(-1, setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, &temp, sizeof(temp)), "Error: failed to setsockopt.");
	errCheck(-1, bind(_server_fd, (struct sockaddr *) &_addr, _addr_len), "Error: failed to bind.");
	errCheck(-1, listen(_server_fd, USER_SIZE), "Error: failed to listen.");

	serverInfo("ircserver listening...");
}


void    Server::serverInfo(char *message)
{
    time_t now = time(0);
	tm *ltm = localtime(&now);

	std::cout << ltm->tm_mday << "." << ltm->tm_mon+1 << "." << ltm->tm_year+1900;
  	std::cout << " " << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << std::endl;
	std::cout << "[" << message << "]" << std::endl;
}

//Getters and Setters

int                 Server::getPort() {return _port;}
int                 Server::getServerfd() {return _server_fd;}
char*               Server::getPassword() {return _password;}
socklen_t           Server::getAddrlen() {return _addr_len;}
struct sockaddr_in  Server::getAddr() {return _addr;}

void                Server::setPort(int port){_port = port;}
void                Server::setServerfd(int server_fd){_server_fd = server_fd;}
void                Server::setPassword(char* password){_password = password;}
void                Server::setAddrlen(socklen_t addrLen){_addr_len = addrLen;}
void                Server::setAddr(struct sockaddr_in addr){_addr = addr;}

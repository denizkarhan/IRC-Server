#include "../headers/Server.hpp"

Server::Server(char **av)
{
    _port = std::atoi(av[1]);
    _password = av[2];
    _socketFd = createSocket();
}

Server::~Server()
{

}

int Server::createSocket()
{
	int (t) = 1;
	struct sockaddr_in servAdrs;

	memset((char *)&servAdrs, 0, sizeof(servAdrs));
	servAdrs.sin_family = AF_INET;
	servAdrs.sin_addr.s_addr = INADDR_ANY;
	servAdrs.sin_port = htons(_port);

	int sockFd = errCheck(socket(AF_INET, SOCK_STREAM, 0), -1, "Error: failed to create socket.");
	errCheck(setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &t, sizeof(t)), -1, "Error: failed to setsockopt.");
	errCheck(bind(sockFd, (struct sockaddr *) &servAdrs, sizeof(servAdrs)), -1, "Error: failed to bind.");
	errCheck(listen(sockFd, 1024), -1, "Error: failed to listen.");

	serverInfo("ircserver listening...");
	return sockFd;
}


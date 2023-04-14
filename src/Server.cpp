#include "../headers/Server.hpp"

Server::Server(char **av) : _usrCount(0) {
	_port = std::atoi(av[1]);
    _password = av[2];

	memset((char *)&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = INADDR_ANY;
	_addr.sin_port = htons(_port);

	_socketFd = createSocket();

	_commands["CAP"] = &Server::cap;
	_commands["QUIT"] = &Server::quit;
	_commands["JOIN"] = &Server::join;
	_commands["NICK"] = &Server::nick;
	_commands["PASS"] = &Server::pass;
	_commands["USER"] = &Server::user;
	_commands["KICK"] = &Server::kick;
	_commands["PART"] = &Server::part;
	_commands["PING"] = &Server::ping;
	_commands["PRIVMSG"] = &Server::privmsg;
	_commands["NOTICE"] = &Server::notice;
	_commands["MODE"] = &Server::mode;
	_commands["WHO"] = &Server::who;
}

Server::~Server() {

	for (std::map<int, Client *>::iterator it = _clients.begin() ; it != _clients.end(); ++it) {
	    delete it->second;
	}
	_clients.clear();

    for (std::map<std::string, Channel *>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
        delete it->second;
    }
    _channels.clear();
}

int Server::createSocket() {
	int t = 1;

	_socketFd = errCheck(-1, socket(AF_INET, SOCK_STREAM, 0), "Error: failed to create socket.");
	errCheck(-1, setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &t, sizeof(t)), "Error: failed to setsockopt.");
	errCheck(-1, bind(_socketFd, (struct sockaddr *) &_addr, (socklen_t)sizeof(_addr)), "Error: failed to bind.");
	errCheck(-1, listen(_socketFd, 1024), "Error: failed to listen.");

	_pollfds.push_back((pollfd){_socketFd, POLLIN, 0});

	serverInfo("ircserver listening...");
	return _socketFd;
}

void	Server::loop() {
	while (1)
	{
		errCheck(-1, poll(_pollfds.begin().base(), _pollfds.size(), -1), "Poll Failed");
		for (size_t i = 0 ; i < _pollfds.size() ; i++)
		{
			if (_pollfds[i].revents & POLLIN)
			{
				if (_pollfds[i].fd == _socketFd)
				{
					newConnection();
					break ;
				}
				readMessage(_pollfds[i].fd);
			}
		}
	}
}

void	Server::newConnection()
{
	int fd;
	sockaddr_in s_address = {};
	socklen_t s_size = sizeof(s_address);

	fd = errCheck(-1, accept(_socketFd, (sockaddr *) &s_address, &s_size), "Error: accept failed!\n");
	_pollfds.push_back((pollfd){fd, POLLIN, 0});

	char hostname[1024];
	if (getnameinfo((struct sockaddr *) &s_address, sizeof(s_address), hostname, 1024, NULL, 0, NI_NUMERICSERV) != 0)
		{std::cerr << "Error: failed to get client hostname!\n"; return ;}

	Client *client = new Client(fd, ntohs(s_address.sin_port));
	_clients.insert(std::make_pair(fd, client));
	_usrCount += 1;

	serverInfo("User has connected");
}

void Server::readMessage(int fd) {
	std::string	message;
	char buffer[100] = {0};

	while (!std::strstr(buffer, "\r\n"))
	{
		memset(buffer, 0, 100);
		errCheck(-1, recv(fd, buffer, 100, 0), "Error: Failed recv function!\n");
		message.append(buffer);
	}

	std::stringstream newMessage(message);
	std::string temp;

	while (std::getline(newMessage, temp))
	{
		temp = temp.substr(0, temp.length() - (temp[temp.length() - 1] == '\r'));
		std::string commandName = temp.substr(0, temp.find(' '));

		std::vector<std::string> arguments;

		std::string buf;
		std::stringstream args(temp.substr(commandName.length(), temp.length()));
		std::string	buf2;
		int flag = 0;

		while (args >> buf)
		{
			if (buf[0] == ':' || flag)
			{
				buf2 += buf + " ";
				flag = 1;
			}
			else
				arguments.push_back(buf);
		}
		if (buf2.size() > 0)
			arguments.push_back(buf2);
		
		arguments.insert(arguments.begin(), commandName);
		
		// for (size_t i = 0 ; i < arguments.size() ; i++)
		// 	std::cout << "*" << arguments[i] << "*\n";

		if (arguments[0] == "USER" && _clients[fd]->getStatus() != 1)
		{
			std::vector<std::string> msg;
			std::string errMsg = "ERROR :Error either password was not given or the nick was in use";
			ft_write(fd, errMsg);
			msg.push_back("Passwords didn't match");
			quit(fd, msg);
			break ;
		}

		if (_commands.find(arguments[0]) != _commands.end())
			(this->*_commands[toUpper(arguments[0])])(fd, arguments);
		else
			ft_write(fd, ERR_UNKNOWNCOMMAND(_clients[fd]->getNickName(), commandName));
	}
}

void	Server::casting(int _fd, std::vector<Client *> _clients, const std::string &message)
{
    for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
        ft_write(_fd, message);
}

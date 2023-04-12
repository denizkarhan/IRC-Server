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
	_commands["PONG"] = &Server::pong;
	_commands["PRIVMSG"] = &Server::privmsg;
	_commands["NOTICE"] = &Server::notice;
	_commands["MODE"] = &Server::notice;
	_commands["WHO"] = &Server::who;
}

Server::~Server() {
	_channels.erase(_channels.begin(), _channels.end());
	_clients.erase(_clients.begin(), _clients.end());
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
	while (1) {		
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

	char message[1000];
	sprintf(message, "User%d: %d has connected.", _usrCount, client->getPort());
	serverInfo(message);
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
		temp = temp.substr(0, temp.length() - (temp[temp.length() - 1] == '\r')); // Komut sonu değilse olduğu gibi al
		std::string commandName = temp.substr(0, temp.find(' ')); // ana komut ismini alıyorum (boşluktan önce)

		std::vector<std::string> arguments; // Komutların argümanlarını tutuyorum

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
		
		arguments.insert(arguments.begin(), commandName); // Argümanları aldığım komutların senin fonksiyon map'ine uyarlamak için argümanların başına yukarıdan aldığım commandName'i ekledim
		
		if (arguments[0] == "USER" && _clients[fd]->getStatus() != 1)
		{
			std::vector<std::string> msg;
			std::string errMsg = "ERROR :Passwords didn't match";
			ft_write(fd, errMsg);
			msg.push_back("Passwords didn't match");
			quit(fd, msg);
			break ;
		}

		if (_commands.find(arguments[0]) != _commands.end())
			(this->*_commands[toUpper(arguments[0])])(fd, arguments); // İstenen adda bir fonksiyonumuz varsa fonksiyona gidiyorum yoksa command not found.
		else
			std::cout << "Error: command not found!\n";
	}
}

void    Server::serverInfo(std::string message) {
    time_t now = time(0);
	tm *ltm = localtime(&now);

	std::cout << ltm->tm_mday << "." << ltm->tm_mon+1 << "." << ltm->tm_year+1900;
  	std::cout << " " << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << std::endl;
	std::cout << "[" << message << "]" << std::endl;
}

#include "../headers/Server.hpp"

Server::Server(char **av) : _usrCount(1)
{
    _port = std::atoi(av[1]);
    _password = av[2];
	memset((char *)&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = INADDR_ANY;
	_addr.sin_port = htons(_port);
	_addrLen = sizeof(_addr);
    _socketFd = createSocket();
	_map["QUIT"] = &Server::quit;
	_map["JOIN"] = &Server::join;
	_map["CAP"] = &Server::welcome;
	_map["NICK"] = &Server::nick;
	_map["PASS"] = &Server::pass;
	_map["USER"] = &Server::user;
}

Server::~Server()
{

}

int Server::createSocket()
{
	int t = 1;

	_socketFd = errCheck(-1, socket(AF_INET, SOCK_STREAM, 0), "Error: failed to create socket.");
	errCheck(-1, setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &t, sizeof(t)), "Error: failed to setsockopt.");
	errCheck(-1, bind(_socketFd, (struct sockaddr *) &_addr, (socklen_t)_addrLen), "Error: failed to bind.");
	errCheck(-1, listen(_socketFd, 1024), "Error: failed to listen.");

	_pollFds[0].fd = _socketFd;
	_pollFds[0].events = POLLIN;

	for (int i = 1; i < USER_MAX; i++)
		_pollFds[i].fd = -1;
	serverInfo("ircserver listening...");
	return _socketFd;
}

void	Server::loop() {
	while (1)
	{
		errCheck(-1, poll(_pollFds, _usrCount, -1), "Poll Failed");
		if (_pollFds[0].revents == POLLIN)
		{
			// New User connection
			int clientFd = errCheck(-1, accept(_socketFd, (struct sockaddr *) &_addr, \
				(socklen_t*)&_addrLen), "Accept Failed");
			for (int i = 1; i < USER_MAX; i++) {
				if (_pollFds[i].fd == -1){
					_pollFds[i].fd = clientFd;
					_pollFds[i].events = POLLIN;
					_usrCount++;
					break ;
				}
			}
		}
		else
		{
			for (int i = 1; i < USER_MAX; i++){
				if (_pollFds[i].fd == -1)
					continue;
				if (_pollFds[i].revents & POLLIN)
					readMessage(_pollFds[i].fd);
			}
		}
	}
}

void	Server::quit(int fd, std::vector<std::string> token){
	std::cout << "QUIT Function started\n";
	
	close(_pollFds[fd].fd);
	_pollFds[fd].fd = -1;
	std::string msg = _clients[fd]->getNickName() + " successfully quitted\r\n";
	send(fd, msg.c_str(), msg.size(), 0);
	for (int i = 0 ; i < _clients[fd]->_channels.size() ; i++)
	{
		msg = _clients[fd]->getNickName() + " left the " + _clients[fd]->_channels[i]->getName() + "\r\n";
		send(fd, msg.c_str(), msg.size(), 0);
		_clients[fd]->_channels[i]->leftTheChannel(_clients[fd]);
	}	
	_clients.erase(fd);
	_usrCount--;
}

void	Server::welcome(int fd, std::vector<std::string> tokens) {
	std::cout << "New client\n";
	
	Client	*newClient = new Client(fd, _port);

	newClient->setPassword(tokens[4]);
	newClient->setNickName(tokens[7]);
	newClient->setUserName(tokens[10]);

	_clients[fd] = newClient;

	std::string welcome_msg = "***Welcome to mhaksal and dkarhan's irc server***\r\n";
	errCheck(-1, send(fd, welcome_msg.c_str(), welcome_msg.size(), 0), "Send failed");
}

void	Server::pass(int fd, std::vector<std::string> token){
	std::cout << "PASS Function started\n";

	std::string msg = "ERROR :Closing Link: [client IP address] (Incorrect password)\r\n";
	if (token[1] != _password){
		send(fd, msg.c_str(), msg.size(), 0);
	} else {
		//TO DO Belki ekstra bir şeylere ihtiyaç olur
	}
}

void	Server::nick(int fd, std::vector<std::string> token){
	std::cout << "NICK Function started\n";
	std::string msg;

	if (token.empty() || token[0].empty() || token[1].empty())
	{
		msg = "User with alias" + token[1] + "not found\r\n";
		send(fd, msg.c_str(), msg.size(), 0);
		return;
	}

	std::map<int, Client*>::iterator it;
	for (it = _clients.begin() ; it != _clients.end() ; it++)
	{
		if (it->second->getNickName() == _clients[fd]->getNickName())
		{
			if (token[1] != _clients[fd]->getNickName())
			{
				msg = "[" + _clients[fd]->getNickName() + "] yeni nick: [" + token[1] + "]\r\n";
				send(fd, msg.c_str(), msg.size(), 0);
				it->second->setNickName(token[1]);
			}
			else
			{
				msg = "Değiştirmek istediğiniz nick şimdiki isminizden farklı olmalıdır!\r\n";
				send(fd, msg.c_str(), msg.size(), 0);
			}
			break;
		}
	}
}

void	Server::user(int fd, std::vector<std::string> token) {
	std::cout << "USER Function started\n";
	
	std::string msg;
	
	if (token.empty() || token[0].empty() || token[1].empty())
	{
		msg = "Username bulunamadı!\r\n";
		send(fd, msg.c_str(), msg.size(), 0);
		return ;
	}
}

void	Server::join(int fd, std::vector<std::string> token)
{
	std::cout << "JOIN Function started.\n";

	std::string	msg;
	if (token.empty() || token[1].empty()) {
		msg = "JOIN komutunda yeterli argüman yok!\r\n";
		send(fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	if (_channels.find(token[1]) == _channels.end())
	{
		if (token[1][0] != '#')
			token[1] = "#" + token[1];
		msg = token[1] + " kanalı oluşturuluyor\r\n";
		send(fd, msg.c_str(), msg.size(), 0);
		_channels[token[1]] = new Channel(_clients[fd], token[1], token.size() > 1 ? token[2] : "");
	}

	if (_channels[token[1]]->getClientCount() >= _channels[token[1]]->getMaxClientCount())
		msg = "Kanaldaki kullanıcı sayısı doldu!\r\n";
	else if (!(_channels[token[1]]->getPassword().empty()) && _channels[token[1]]->getPassword() != token[2])
		msg = "Kanala girilemedi, parola hatası!\r\n";
	else
	{
		if (_channels[token[1]]->getClientCount() == 0)
		{
			// Admin girişi
			msg = "Admin girişi\r\n";
			_channels[token[1]]->setAdmin(_clients[fd]);
			_channels[token[1]]->_channelClients.push_back(_clients[fd]);
			_clients[fd]->_channels.push_back(_channels[token[1]]);
		}
		else if (_channels[token[1]]->getPassword().empty() || _channels[token[1]]->getPassword() == token[2])
		{
			// Üye girişi
			msg = "Üye girişi\r\n";
			_channels[token[1]]->_channelClients.push_back(_clients[fd]);
			_clients[fd]->_channels.push_back(_channels[token[1]]);
		}
	}
	send(fd, msg.c_str(), msg.size(), 0);
}

void Server::readMessage(int fd){
	char		buffer[BUFFER_SIZE];
	
	memset(buffer, 0, BUFFER_SIZE);
	while (!std::strstr(buffer, "\r\n"))
	{
		memset(buffer, 0, BUFFER_SIZE);
		errCheck(-1, read(fd, buffer, BUFFER_SIZE), "Error receiving the message");
		std::string	str(buffer);
		while (str.find_first_of("\r\n") != std::string::npos)
		{
			std::vector<std::string> tokens = tokenize(str);

			if (std::strstr(tokens[0].c_str(), "CAP"))
			{
				welcome(fd, tokens);
				return ;
			}
			else if (_map.find(tokens[0]) != _map.end())
				(this->*_map[tokens[0]])(fd, tokens);
			
			int i = str.find_first_of("\r\n");
			str = str.substr(i + 1);
		}
	}
}

void    Server::serverInfo(std::string message)
{
    time_t now = time(0);
	tm *ltm = localtime(&now);

	std::cout << ltm->tm_mday << "." << ltm->tm_mon+1 << "." << ltm->tm_year+1900;
  	std::cout << " " << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << std::endl;
	std::cout << "[" << message << "]" << std::endl;
}

//Getters and Setters

int                 Server::getPort() {return _port;}
int                 Server::getServerfd() {return _socketFd;}
char*               Server::getPassword() {return _password;}
int			        Server::getAddrlen() {return _addrLen;}
struct sockaddr_in  Server::getAddr() {return _addr;}

void                Server::setPort(int port){_port = port;}
void                Server::setServerfd(int server_fd){_socketFd = server_fd;}
void                Server::setPassword(char* password){_password = password;}
void                Server::setAddrlen(int addrLen){_addrLen = addrLen;}
void                Server::setAddr(struct sockaddr_in addr){_addr = addr;}
#include "../headers/Server.hpp"

Server::Server(char **av) : _usrCount(1) {
    _port = std::atoi(av[1]);
    _password = av[2];

	memset((char *)&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = INADDR_ANY;
	_addr.sin_port = htons(_port);
    
	_socketFd = createSocket();

	_commands["QUIT"] = &Server::quit;
	_commands["JOIN"] = &Server::join;
	_commands["CAP"] = &Server::welcome;
	_commands["NICK"] = &Server::nick;
	_commands["PASS"] = &Server::pass;
	_commands["USER"] = &Server::user;
}

Server::~Server() {}

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

		for (int i = 0 ; i < _pollfds.size() ; i++)
		{
			if (_pollfds[i].revents == POLLIN)
			{
				if (_pollfds[i].fd == _socketFd)
				{
					newConnection();
					break ;
				}
				readMessage(_pollfds[i].fd);
			}

			if (_pollfds[i].revents == POLLHUP)
			{
				newDisconnection(_pollfds[i].fd);
				break ;
			}
		}
	}
}

void	Server::newConnection()
{
	int fd;
	sockaddr_in s_address = {};
	socklen_t s_size = sizeof(s_address);

	fd = accept(_socketFd, (sockaddr *) &s_address, &s_size);
	if (fd < 0)
		{std::cerr << "Error: accept failed!\n"; return ;}

	_pollfds.push_back((pollfd){fd, POLLIN, 0});

	char hostname[NI_MAXHOST];
	if (getnameinfo((struct sockaddr *) &s_address, sizeof(s_address), hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV) != 0)
		{std::cerr << "Error: failed to get client hostname!\n"; return ;}

	Client *client = new Client(fd, ntohs(s_address.sin_port));
	_clients.insert(std::make_pair(fd, client));

	char message[1000];
	serverInfo(message);
	std::string welcome_msg = "***Welcome to mhaksal and dkarhan's irc server***\r\n";
	errCheck(-1, send(fd, welcome_msg.c_str(), welcome_msg.size(), 0), "Send failed");
}

void	Server::newDisconnection(int fd)
{
	try {
		Client *client = _clients.at(fd);
		client->leave();

		char message[1000];
		serverInfo(message);

		_clients.erase(fd);

		for (std::vector<pollfd>::iterator it = _pollfds.begin(); it != _pollfds.end(); it++) {
			if (it->fd != fd)
				continue;
			_pollfds.erase(it);
			close(fd);
			break;
		}
		delete client;
	}
	catch (const std::out_of_range &ex) {}
}

void Server::readMessage(int fd) {
	std::string	message;
	char buffer[100];

	memset(buffer, 0, 100);
	while (!std::strstr(buffer, "\r\n"))
	{
		memset(buffer, 0, 100);
		if (recv(fd, buffer, 100, 0) < 0)
			std::cerr << "Error: Failed recv function!\n";
		message.append(buffer);
	}
	
	std::stringstream newMessage(message);
	std::string temp;

	while (std::getline(newMessage, temp))
	{
		temp = temp.substr(0, temp.length() - (temp[temp.length() - 1] == '\r')); // Komut sonu değilse olduğu gibi al
		std::string commandName = temp.substr(0, temp.find(' ')); // ana komut ismini alıyorum (boşluktan önce)

		std::vector<std::string> arguments; // Komutların argümanlarını bunda tutuyorum

		std::string buf;
		std::stringstream args(temp.substr(commandName.length(), temp.length()));

		while (args >> buf)
			arguments.push_back(buf);
		arguments.insert(arguments.begin(), commandName); // Argümanları aldığım komutların senin fonksiyon map'ine uyarlamak için argümanların başına yukarıdan aldığım commandName'i ekledim

		if (_commands.find(arguments[0]) != _commands.end())
			(this->*_commands[arguments[0]])(fd, arguments); // İstenen adda bir fonksiyonumuz varsa fonksiyona gidiyorum yoksa command not found.
		else
			std::cout << "Error: command not found!\n";
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

void	Server::quit(int fd, std::vector<std::string> token) {
	std::cout << "QUIT Function started\n";
	
	close(_pollfds[fd].fd);
	_pollfds[fd].fd = -1;
	std::string msg = _clients[fd]->getNickName() + " successfully quitted\r\n";
	send(fd, msg.c_str(), msg.size(), 0);
	for (int i = 0 ; i < _clients[fd]->_channels.size() ; i++) // client serverdan ayrılırken bulunduğu tüm kanallardan ayrılmalı.
	{
		msg = _clients[fd]->getNickName() + " left the " + _clients[fd]->_channels[i]->getName() + "\r\n";
		send(fd, msg.c_str(), msg.size(), 0);
		_clients[fd]->_channels[i]->leftTheChannel(_clients[fd]);
	}	
	_clients.erase(fd); // server'dan kullanıcıyı sildim.
	_usrCount--;
}

void	Server::welcome(int fd, std::vector<std::string> tokens) {
	std::cout << "WELCOME Function client\n";
	
	Client	*newClient = new Client(fd, _port);
	_clients[fd] = newClient;
	std::string welcome_msg = "***Welcome to mhaksal and dkarhan's irc server***\r\n";
	errCheck(-1, send(fd, welcome_msg.c_str(), welcome_msg.size(), 0), "Send failed");
}

void	Server::pass(int fd, std::vector<std::string> token) {
	std::cout << "PASS Function started\n";

	std::string msg = "ERROR :Closing Link: [client IP address] (Incorrect password)\r\n";
	if (token.size() < 2) // Sadece PASS
		msg = "Error: Password information is missing\r\n";
	if (std::strstr(this->getPassword(), token[1].c_str())) { // Şifreyi bilen birisi server'a girebilir ileride status durumu olacak, şimdilik 1 ise server'a girmiştir.
		_clients[fd]->setStatus(1);
		return ;
	}
	send(fd, msg.c_str(), msg.size(), 0);
}

void	Server::nick(int fd, std::vector<std::string> token) {
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

void	Server::join(int fd, std::vector<std::string> token) {
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
			_channels[token[1]]->setClientCount(_channels[token[1]]->getClientCount() + 1);
		}
		else if (_channels[token[1]]->getPassword().empty() || _channels[token[1]]->getPassword() == token[2])
		{
			// Üye girişi
			msg = "Üye girişi\r\n";
			_channels[token[1]]->_channelClients.push_back(_clients[fd]);
			_clients[fd]->_channels.push_back(_channels[token[1]]);
			_channels[token[1]]->setClientCount(_channels[token[1]]->getClientCount() + 1);
		}
	}
	std::cout << "Kullanıcı sayısı:	" << _channels[token[1]]->getClientCount() << std::endl;
	send(fd, msg.c_str(), msg.size(), 0);
}

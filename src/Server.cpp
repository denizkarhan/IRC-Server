#include "../headers/Server.hpp"

Server::Server(char **av) : _usrCount(0) {
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
	while (1) {		
		errCheck(-1, poll(_pollfds.begin().base(), _pollfds.size(), -1), "Poll Failed");
		
		for (pollfd pfd : _pollfds)
		{
			if (pfd.revents & POLLIN)
			{
				if (pfd.fd == _socketFd)
				{
					newConnection();
					break ;
				}
				readMessage(pfd.fd);
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

		while (args >> buf)
			arguments.push_back(buf);
		arguments.insert(arguments.begin(), commandName); // Argümanları aldığım komutların senin fonksiyon map'ine uyarlamak için argümanların başına yukarıdan aldığım commandName'i ekledim

		// for (std::string args : arguments)
		// 	std::cout << args << std::endl;

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

void	Server::quit(int fd, std::vector<std::string> token) { // send komutları quit fonksiyonuna gelir gelmez kullanıcının fd'si kapandığı için hata veriyor, neden kapanıyor?
	std::cout << "QUIT Function started\n";

	Client *clientPtr = _clients.at(fd);
	std::string msg;

	// Kullanıcı /quit ile çıkarken argüman olarak ayrılık mesajı da verebilir Verdiği argümanları ' ' ile birleştirip client'e göndermeliyiz.
	// msg = _clients[fd]->getNickName() + " successfully quitted\r\n"; send(fd, msg.c_str(), msg.size(), 0);

	for (int i = 0 ; i < _pollfds.size() ; i++)
	{
		if (fd == _pollfds[i].fd)
		{
			close(_pollfds[i].fd);
			_pollfds.erase(_pollfds.begin() + i);
		}
	}
	for (int i = 0 ; i < _clients[fd]->_channels.size() ; i++) // client serverdan ayrılırken bulunduğu tüm kanallardan ayrılmalı.
	{
		msg = _clients[fd]->getNickName() + " left the " + _clients[fd]->_channels[i]->getName() + "\r\n";
		std::cout << msg;
		_clients[fd]->_channels[i]->leftTheChannel(_clients[fd]);
		if (_clients[fd]->_channels[i]->getClientCount() == 0)
		{
			std::cout << "Kanal Siliniyor...\n";
			_channels.erase(_channels.find(_clients[fd]->_channels[i]->getName()));
		}
		// send(fd, msg.c_str(), msg.size(), 0);
	}
	_clients.erase(fd); // server'dan kullanıcıyı sildim.
	delete clientPtr;
	_usrCount--;
}

void	Server::welcome(int fd, std::vector<std::string> tokens) {
	// std::cout << "WELCOME Function client\n";

	std::string welcome_msg = "***Welcome to mhaksal and dkarhan's irc server***\r\n";
	errCheck(-1, send(fd, welcome_msg.c_str(), welcome_msg.size(), 0), "Send failed");
}

void	Server::pass(int fd, std::vector<std::string> token) {
	// std::cout << "PASS Function started\n";

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
	// std::cout << "NICK Function started\n";
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
			if (token[1] == _clients[fd]->getNickName())
			{
				msg = "Değiştirmek istediğiniz nick şimdiki isminizden farklı olmalıdır!\r\n";
				send(fd, msg.c_str(), msg.size(), 0);
			}
			else
			{
				msg = "[" + _clients[fd]->getNickName() + "] yeni nick: [" + token[1] + "]\r\n";
				send(fd, msg.c_str(), msg.size(), 0);
				it->second->setNickName(token[1]);
			}
			break;
		}
	}
}

void	Server::user(int fd, std::vector<std::string> token) {
	// std::cout << "USER Function started\n";

	std::string msg;

	if (token.empty() || token[0].empty() || token[1].empty())
	{
		msg = "Username bulunamadı!\r\n";
		send(fd, msg.c_str(), msg.size(), 0);
		return ;
	}
}

void	Server::join(int fd, std::vector<std::string> token) {
	// std::cout << "JOIN Function started.\n";

	std::string	msg;
	if (token.empty() || token[1].empty()) {
		msg = "JOIN komutunda yeterli argüman yok!\r\n";
		send(fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	if (token[1][0] != '#')
		token[1] = "#" + token[1];
	
	if (token[1].size() < 2) // "/join #" kontrolü
	{
		msg = "Kanal belirtmediniz!\r\n";
		send(fd, msg.c_str(), msg.size(), 0);
		return ;
	}
	
	if (_channels.find(token[1]) == _channels.end())
	{
		// Admin girişi, kanal oluşturuluyor.
		msg = token[1] + " kanalı oluşturuluyor\r\n"; send(fd, msg.c_str(), msg.size(), 0);
		msg = "Admin girişi\r\n"; send(fd, msg.c_str(), msg.size(), 0);
		_channels[token[1]] = new Channel(_clients[fd], token[1], token.size() > 2 ? token[2] : "");
		_clients[fd]->_channels.push_back(_channels[token[1]]);
		return ;
	}

	for (int i = 0 ; i < _clients[fd]->_channels.size() ; i++)
	{
		if (_clients[fd]->_channels[i]->getName() == token[1])
		{
			msg = "Zaten bu kanaldasınız!\r\n";
			send(fd, msg.c_str(), msg.size(), 0);
			return ;
		}
	}
	if (_channels[token[1]]->getClientCount() >= _channels[token[1]]->getMaxClientCount())
		msg = "Kanaldaki kullanıcı sayısı doldu!\r\n";
	else if (!(_channels[token[1]]->getPassword().empty()) && _channels[token[1]]->getPassword() != token[2])
		msg = "Kanala girilemedi, parola hatası!\r\n";
	else
	{
		// Üye girişi
		msg = "Üye girişi\r\n";
		_channels[token[1]]->_channelClients.push_back(_clients[fd]);
		_clients[fd]->_channels.push_back(_channels[token[1]]);
		_channels[token[1]]->setClientCount(_channels[token[1]]->getClientCount() + 1);
	}
	std::cout << "Kullanıcı sayısı:	" << _channels[token[1]]->getClientCount() << std::endl;
	send(fd, msg.c_str(), msg.size(), 0);
}

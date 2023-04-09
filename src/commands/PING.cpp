#include "../../headers/Server.hpp"

std::string getPrefix(std::string const &_nickname, std::string const &_username, std::string const &_hostname)
{
	std::string nickname = _nickname;

	if (!(_username.empty()))
		nickname += '!' + _username;
	if (!(_hostname.empty()))
		nickname += '@' + _hostname;
	return (nickname);
}

void sendWrite(int fd, std::string const &msg)
{
	std::string temp = msg + "\r\n";
    errCheck(-1, send(fd, temp.c_str(), temp.length(), 0), "Error: failed sending message to client.\n");
}

void	Server::ping(int fd, std::vector<std::string> token)
{
    if (token.empty() or token.size() < 2) {
		std::string msg = "Daha fazla parametre gerekli!\r\n";
        send(fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	sendWrite(fd, ":" + getPrefix(_clients[fd]->getNickName(), _clients[fd]->getUserName(), _clients[fd]->getHostName()) + " PONG :" + token.at(1));
}


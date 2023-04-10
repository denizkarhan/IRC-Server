#include "../../headers/Server.hpp"

void	Server::cap(int fd, std::vector<std::string> tokens) {
	(void)tokens;
	std::string msg;
	for (command_map::iterator it = _commands.begin(); it != _commands.end(); it++)
		msg += "/" + it->first + "\n";
	msg += "\r\n";
	errCheck(-1, send(fd, msg.c_str(), msg.size(), 0), "Send failed");
}

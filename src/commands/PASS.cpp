#include "../../headers/Server.hpp"

void	Server::pass(int fd, std::vector<std::string> token)
{
	if (token.size() != 2)
		ft_write(fd,"Error: Password information is missing!");
	else if (this->getPassword() == token[1])
		_clients[fd]->setStatus(1);
}

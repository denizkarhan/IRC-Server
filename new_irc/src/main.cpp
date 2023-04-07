#include "../headers/Utils.hpp"
#include "../headers/Server.hpp"

int	main(int ac, char **av)
{
	if (ac != 3)
	 	exitErr("Usage: ./ft_irc <port> <password>");
	Server server(av);
	server.loop();
}

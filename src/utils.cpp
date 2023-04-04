#include "../headers/Utils.hpp"

void    serverInfo(const std::string &message)
{
	time_t now = time(0);
	tm *ltm = localtime(&now);

	std::cout << ltm->tm_mday << "." << ltm->tm_mon+1 << "." << ltm->tm_year+1900;
  	std::cout << " " << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << std::endl;
	std::cout << "[" << message << "]" << std::endl;
};

void	exitErr(std::string msg)
{
	std::cerr << msg << "\n";
	exit(1);
}

int	errCheck(int cnd, int res, std::string msg)
{
	if (cnd == res)
		exitErr(msg);
	return cnd;
}

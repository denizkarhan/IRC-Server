#include "../headers/Utils.hpp"

void    serverInfo(std::string const &message) {
	time_t now = time(0);
	tm *ltm = localtime(&now);

	std::cout << ltm->tm_mday << "." << ltm->tm_mon+1 << "." << ltm->tm_year+1900;
  	std::cout << " " << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << std::endl;
	std::cout << "[" << message << "]" << std::endl;
};

void	exitErr(std::string msg) {
	std::cerr << msg << "\n";
	exit(1);
}

int	errCheck(int cnd, int res, std::string msg) {
	if (cnd == res)
		exitErr(msg);
	return res;
}

void	ft_write(int _fd, const std::string &message) {
    std::string msg = message + "\r\n";
	errCheck(-1, send(_fd, msg.c_str(), msg.length(), 0), "Error: failed send message!");
}

std::string toUpper(std::string str) {
    for (size_t i = 0; i < str.length(); ++i)
        str[i] = toupper(str[i]);
    return str;
}

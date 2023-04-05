#ifndef FT_IRC_HPP
# define FT_IRC_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sys/poll.h>
#include <sys/socket.h>
#include <algorithm>
#include <arpa/inet.h>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <netdb.h>
#include <poll.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 4096
#define USER_SIZE 100

void	*errCheck(void *cnd, void *res, std::string msg);
int	    errCheck(int cnd, int res, std::string msg);
void	exitErr(std::string msg);
void    serverInfo(const std::string &message);

#endif
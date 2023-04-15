#include "../../headers/Server.hpp"

void	Server::join(int fd, std::vector<std::string> token) {
	std::string	msg;

	if (token.size() < 2) {
		_clients[fd]->clientMsgSender(fd, ERR_NEEDMOREPARAMS(_clients[fd]->getNickName(), "PASS")); return ; }

	token[1] = (token[1][0] != '#' ? "#" + token[1] : token[1]);
	for (size_t i = 0 ; i < _clients[fd]->_channels.size() ; i++) {
		if (_clients[fd]->_channels[i]->getName() == token[1]) {
			msg = "You're already on this channel!"; ft_write(fd, msg); return ; } }

	if (_channels.find(token[1]) == _channels.end())
	{
		msg = token[1] + " creating channel..."; ft_write(fd, msg);
		msg = "Admin login"; ft_write(fd, msg);
		_channels[token[1]] = new Channel(_clients[fd], token[1], token.size() > 2 ? token[2] : "");
		_clients[fd]->_channels.push_back(_channels[token[1]]);
		_clients[fd]->casting(fd, _channels[token[1]]->_channelClients, RPL_JOIN(_clients[fd]->getPrefixName(), token[1]) + " :" + _clients[fd]->getNickName());
		return ;
	}
	
	if (_channels[token[1]]->getClientCount() >= _channels[token[1]]->getMaxClientCount()) {
		msg = "Channel is full!"; ft_write(fd, msg); }
	else if (!(_channels[token[1]]->getPassword().empty()) && _channels[token[1]]->getPassword() != token[2]) {
		msg = "Failed, password error!"; ft_write(fd, msg); }
	else
	{
		_channels[token[1]]->_channelClients.push_back(_clients[fd]);
		_clients[fd]->_channels.push_back(_channels[token[1]]);
		_channels[token[1]]->setClientCount(_channels[token[1]]->getClientCount() + 1);
		_clients[fd]->casting(fd, _channels[token[1]]->_channelClients, RPL_JOIN(_clients[fd]->getPrefixName(), token[1]) + " :" + _clients[fd]->getNickName());
	}
}

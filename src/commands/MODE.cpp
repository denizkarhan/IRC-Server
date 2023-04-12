#include "../../headers/Server.hpp"

void broadcasting(const std::string &message, std::vector<Client *> _clients)
{
	for (size_t i = 0 ; i < _clients.size() ; i++)
		ft_write(_clients[i]->getFd(), message);
}

void	Server::mode(int fd, std::vector<std::string> token)
{
    token.erase(token.begin());
	if (token.size() < 2) {
		_clients[fd]->clientMsgSender(fd, ERR_NEEDMOREPARAMS(_clients[fd]->getNickName(), "MODE"));
		return;
	}

	std::string target = token.at(0);

	Channel *channel = _channels[target];
	if (!channel) { // Kanal bulunamadı
		_clients[fd]->clientMsgSender(fd, ERR_NOSUCHCHANNEL(_clients[fd]->getNickName(), target));
		return;
	}

	if (channel->getAdmin() != _clients[fd]) { // Kanalın admini değilsiniz
		_clients[fd]->clientMsgSender(fd, ERR_CHANOPRIVSNEEDED(_clients[fd]->getNickName(), target));
		return;
	}

	int i = 0;
	int p = 2;
	char c;

	while ((c = token[1][i])) {

		char prevC = (i > 0 ? token[1][i - 1] : '\0');
		bool active = (prevC == '+');

		switch (c) {

			case 'n': {
				channel->setN(active);
				broadcasting(RPL_MODE(_clients[fd]->getPrefixName(), channel->getName(), (active ? "+n" : "-n"), ""), _channels[target]->_channelClients);
				break;
			}

			case 'l': {
				channel->setL(active ? std::atoi(token[p].c_str()) : 0);
				broadcasting(RPL_MODE(_clients[fd]->getPrefixName(), channel->getName(), (active ? "+l" : "-l"), (active ? token[p] : "")), _channels[target]->_channelClients);
				p += active ? 1 : 0;
				break;
			}

			case 'k': {
				channel->setK(active ? token[p] : "");
				broadcasting(RPL_MODE(_clients[fd]->getPrefixName(), channel->getName(), (active ? "+k" : "-k"), (active ? token[p] : "")), _channels[target]->_channelClients);
				p += active ? 1 : 0;
				break;
			}

			default:
				break;
		}

		i++;
    }
}

// "k" Modu (Kanal Anahtarı): Kanalın şifre korumalı olmasını sağlar. Sadece doğru kanal anahtarına sahip kullanıcılar, kanala katılabilir ve mesaj gönderebilir.
// Kullanımı: MODE #kanal +k [kanal_anahtarı]
// Örnek: MODE #oda +k sifre123

// Not: "[kanal_anahtarı]" yerine, kanalın kullanılmasını istediğiniz anahtarı belirtmelisiniz. Kanala katılmak isteyen kullanıcılar, bu anahtarı kullanarak kanala giriş yapmalıdır.

// "l" Modu (Kanal Sınırlı Üye Sayısı): Kanalın sınırlı bir kullanıcı sayısına sahip olmasını sağlar. Belirli bir kullanıcı sayısını aştığında, yeni kullanıcıların kanala girişi engellenir.
// Kullanımı: MODE #kanal +l [maksimum_kullanıcı_sayısı]
// Örnek: MODE #oda +l 20

// Not: "[maksimum_kullanıcı_sayısı]" yerine, kanalda bulunmasını istediğiniz maksimum kullanıcı sayısını belirtmelisiniz.

// "n" Modu (No External Messages): Kanalın yalnızca kanal kullanıcıları tarafından görülen mesajlara izin verilmesini sağlar. Kanal dışındaki kullanıcılar, kanalda görülen mesajları göremez.

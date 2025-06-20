#include "../../includes/Server.hpp"

int		Server::handleNick(Client* client, const std::vector<std::string>& params)
{
	bool valid = true;
	std::string nickname = params[0];
	std::string announce;

	if(params.empty() || client->getPassword().empty() || client->getRemoveClient() == true)
		return (client->setRemoveClient(true),1);
	if (nickname.empty())
		valid = false;
	for (size_t i = 0; i < nickname.length() && valid == true; i++) {
		if (!isprint(nickname[i]))
			valid = false;
	}
	if (valid == false)
		return (this->sendToClient(client, "432 : [" + nickname + "] Erroneous nickname"), client->setRemoveClient(true), 1);
	for (size_t i = 0; i < this->clients.size(); i++)
	{
		if (this->clients[i] == client && this->clients[i]->getNickName() != nickname)
		{
			announce = ":" + client->getNickName() + "!" + client->getUserName() + "@localhost " + "NICK " + nickname;
			this->sendToClient(client, announce);
			this->clients[i]->setNickName(nickname);
			return (0);
		}
		if (this->clients[i] != client && this->clients[i]->getNickName() == nickname)
			return (this->sendToClient(client, "433 : [" + nickname + "] Nickname is already in use"), client->setRemoveClient(true) ,1);
	}
	client->setNickName(nickname);
	return (0);
}

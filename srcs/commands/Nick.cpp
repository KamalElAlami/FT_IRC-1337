#include "../../includes/Server.hpp"

int		Server::handleNick(Client* client, const std::vector<std::string>& params)
{
	std::string announce;
	std::string nickname;
	
	if(client->getPassword().empty() || client->getRemoveClient() == true)
		return (client->setRemoveClient(true),1);
	
	if(params.empty())
		return (this->sendToClient(client, "431 : No nickname given"), client->setRemoveClient(true),1);
	
	nickname = params[0];
	
	for (size_t i = 0; i < nickname.length(); i++) {
		if (!isprint(nickname[i]) || (!isalnum(nickname[i]) && nickname[i] != '-') || isdigit(nickname[0]))
			return (this->sendToClient(client, "432 : Erroneous nickname"), client->setRemoveClient(true), 1);
	}

	for (size_t i = 0; i < this->clients.size(); i++)
	{
		if (this->clients[i] != client && this->clients[i]->getNickName() == nickname)
			return (this->sendToClient(client, "433 : Nickname is already in use"), client->setRemoveClient(true) ,1);
	}

	if (client && client->getNickName() != nickname)
	{
		announce = ":" + client->getNickName() + "!" + client->getUserName() + "@localhost " + " NICK " + nickname;
		this->sendToClient(client, announce);
		client->setNickName(nickname);
		return (0);
	}

	client->setNickName(nickname);
	return (0);
}

#include "../../includes/Server.hpp"

int		Server::handleNick(Client* client, const std::vector<std::string>& params)
{
	std::string announce;
	std::string nickname;
	
	if(client->getRemoveClient() == true)
		return (client->setRemoveClient(true),1);
	
	if (client->getPassword().empty())
		return (this->sendToClient(client, "You must provide <PASS>, <NICK> then <USER> to register."),1);

	if(params.empty())
	{
		if(client->getRegistered() == true)
			return (this->sendToClient(client, "431 : No nickname given"),1);
		return (this->sendToClient(client, "431 : No nickname given (you are now disconnected)"), client->setRemoveClient(true),1);
	}
	
	nickname = params[0];
	
	for (size_t i = 0; i < nickname.length(); i++) {
		if (!isprint(nickname[i]) || (!isalnum(nickname[i]) && nickname[i] != '-') || isdigit(nickname[0]))
		{
			if(client->getRegistered() == true)
				return (this->sendToClient(client, "432 : Erroneous nickname"), 1);
			return (this->sendToClient(client, "432 : Erroneous nickname (you are now disconnected)"), client->setRemoveClient(true), 1);
		}
	}
	
	for (size_t i = 0; i < this->clients.size(); i++)
	{
		if (this->clients[i] != client && this->clients[i]->getNickName() == nickname)
		{
			
			if(client->getRegistered() == true)
				return (this->sendToClient(client, "433 : Nickname is already in use"),1);
			return (this->sendToClient(client, "433 : Nickname is already in use (you are now disconnected)"), client->setRemoveClient(true) ,1);
		}
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

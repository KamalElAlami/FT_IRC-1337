#include "../../includes/Server.hpp"

int Server::handlePass(Client* client, const std::vector<std::string>& params)
{
	if (client->getRegistered() == true)
		return (this->sendToClient(client, "You can't change your password because you are already registered"), 1);

	if (params.empty())
	{
		if (client->getRegistered() == true)
			return (this->sendToClient(client, "461 : Not enough parameters"), 1);
		return (this->sendToClient(client, "461 : Not enough parameters (you are now disconnected)"), client->setRemoveClient(true), 1);
	}
	
	if (params.size() != 1)
	{
		if (client->getRegistered() == true)
			return (this->sendToClient(client, "461 : Too many parameters"), 1);
		return (this->sendToClient(client, "461 : Too many parameters (you are now disconnected)"), client->setRemoveClient(true), 1);
	}

	if (client->getRegistered())
	{
		if (client->getRegistered() == true)
			return (this->sendToClient(client, "462 : You may not reregister"), 1);
		return (this->sendToClient(client, "462 : You may not reregister (you are now disconnected)"), client->setRemoveClient(true), 1);
	}

	if (params[0] != this->Password)
	{
		if (client->getRegistered() == true)
			return (this->sendToClient(client, "464 : Password incorrect"), 1);
		return (this->sendToClient(client, "464 : Password incorrect (you are now disconnected)"), client->setRemoveClient(true), 1);
	}

	client->setPassword(params[0]);
	return (0);
}
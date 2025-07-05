#include "../../includes/Server.hpp"

int Server::handlePass(Client* client, const std::vector<std::string>& params)
{
	if (client->getRegistered() == true)
		return (this->sendToClient(client, "462 : You can't change your password because you are already registered"), 1);

	if (params.empty())
		return (this->sendToClient(client, "461 : Not enough parameters, you are now disconnected"), client->setRemoveClient(true), 1);
	
	if (params.size() != 1)
		return (this->sendToClient(client, "461 : Too many parameters, you are now disconnected"), client->setRemoveClient(true), 1);

	if (client->getRegistered())
		return (this->sendToClient(client, "462 : You may not reregister, you are now disconnected"), client->setRemoveClient(true), 1);

	if (params[0] != this->Password)
		return (this->sendToClient(client, "464 : Password incorrect, you are now disconnected"), client->setRemoveClient(true), 1);

	client->setPassword(params[0]);
	return (0);
}
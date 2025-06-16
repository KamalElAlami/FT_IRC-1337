#include "../../includes/Server.hpp"

int Server::handlePass(Client* client, const std::vector<std::string>& params)
{
	if (params.empty())
		return (this->sendToClient(client, "461 : Not enough parameters"), this->removeClient(client->getClientfd()), 1);

	if (client->getRegistered())
		return (this->sendToClient(client, "462 : You may not reregister"), this->removeClient(client->getClientfd()), 1);

	if (params[0] != this->Password)
		return (this->sendToClient(client, "464 : Password incorrect"), this->removeClient(client->getClientfd()), 1);

	return (client->setPassword(params[0]), 0);
}
#include "../../includes/Server.hpp"

int Server::handlePass(Client* client, const std::vector<std::string>& params)
{
	if (params.empty())
		return (this->sendToClient(client, "461 : Not enough parameters"), 1);

	if (client->getRegistered())
		return (this->sendToClient(client, "462 : You may not reregister"), 1);

	if (params[0] != this->Password)
		return (this->sendToClient(client, "464 : Password incorrect"), 1);

	client->setPassword(params[0]);
	// this->checkRegistration(client);

	return (0);
}
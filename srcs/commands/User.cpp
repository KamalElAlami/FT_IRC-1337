#include "../../includes/Server.hpp"

int	Server::handleUser(Client* client, const std::vector<std::string>& params)
{
	if(params.size() < 4)
		return (this->sendToClient(client, "461 :Not enough parameters"), this->removeClient(client->getClientfd()), 1);
	if (client->getRegistered())
		return (this->sendToClient(client, "462 :You may not reregister"), this->removeClient(client->getClientfd()), 1);
	client->setUserName(params[0]);
	client->setHostName(params[1]);
	client->setRealName(params[2]);
	this->checkRegistration(client);
	return (0);
}
#include "../../includes/Server.hpp"

int	Server::handleUser(Client* client, const std::vector<std::string>& params)
{
	if(client->getRemoveClient() == true)
		return (client->setRemoveClient(true),1);
	
	if (client->getPassword().empty() || client->getNickName().empty())
		return (this->sendToClient(client, "461 : Missing parameters. You must provide <PASS>, <NICK> then <USER> to register."),1);

	if(params.size() != 4)
		return (this->sendToClient(client, "461 : Not enough parameters, you are now disconnected"), client->setRemoveClient(true), 1);
  	if (client->getRegistered())
		return (this->sendToClient(client, "462 : You may not reregister, you are now disconnected"), client->setRemoveClient(true), 1);
	client->setUserName(params[0]);
	client->setHostName(params[1]);
	client->setRealName(params[2]);
	this->checkRegistration(client);
	return (0);
}
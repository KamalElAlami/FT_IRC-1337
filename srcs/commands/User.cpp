#include "../../includes/Server.hpp"

int	Server::handleUser(Client* client, const std::vector<std::string>& params)
{
	// .......................................................
	// static flag i = 0
	// when first connect pass normally and set i tp 1 before the return 
	// next enrty if i != 0 ERR_ALREADYREGISTERED reply and return 
	// static int first_entry = 0;
	// if (first_entry != 0) {
	// 	// Reply ERR_ALREADYREGISTERED;
	// 	return (0);
	// }
	// .......................................................
	if(client->getPassword().empty() || client->getRemoveClient() == true)
		return (client->setRemoveClient(true), 1);
	if(params.size() < 4)
		return (this->sendToClient(client, "461 : Not enough parameters"), client->setRemoveClient(true), 1);
  	if (client->getRegistered())
		return (this->sendToClient(client, "462 : You may not reregister"), client->setRemoveClient(true), 1);
	client->setUserName(params[0]);
	client->setHostName(params[1]);
	client->setRealName(params[2]);
	this->checkRegistration(client);
	// first_entry = 1;
	return (0);
}
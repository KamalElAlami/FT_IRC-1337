#include "../../includes/Server.hpp"

int Server::handlePrivMsg(Client* client, const std::vector<std::string>& params)
{
	std::string message;
	int chanIdx;
	int userIdx;

	if (params.size() != 2)
		return ( this->sendToClient(client, "461 :Not enough parameters"), 1);
	if (params[1].empty() || (params[1].size() == 1 && params[1][0] == ':') )
		return (1);
	message = ":" + client->getNickName() + "!" + client->getUserName() + "@" + client->getHostName() + " PRIVMSG " + params[0] + " :" + params[1] + "\r\n";
	if (params[0][0] == '#')
	{
		chanIdx = this->isChannelExist(params[0]);
		if (chanIdx == -1)
			return (this->sendToClient(client, "401 " + params[0] + " :No such nick/channel"), 1);
		userIdx = this->findUser(client->getNickName(), this->chanPool[chanIdx]->getMembers());
		if (userIdx == -1)	
			return (this->sendToClient(client, "404 " + params[0] + " :Cannot send to channel"), 1);
		this->sendMsgToChannel(client, this->chanPool[chanIdx]->getMembers(), message);
	}
	else
	{
		userIdx = this->findUser(params[0], clients);
		if (userIdx == -1)
			return ( this->sendToClient(client, "401  :No such nick/channel"), 1);
		send(clients[userIdx]->getClientfd(), message.c_str(), message.length(), 0);
	}
	return 0;
}

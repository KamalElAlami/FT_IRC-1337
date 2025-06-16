#include "../../includes/Server.hpp"

int Server::handlePrivMsg(Client* client, const std::vector<std::string>& params)
{
	std::string message;
	int idx;

	if (params.size() != 2)
		return ( this->sendToClient(client, "461 :Not enough parameters"), 1);
	message = ":" + client->getNickName() + "!" + client->getUserName() + "@localhost PRIVMSG " + params[0] + " :" + params[1] + "\r\n";
	if (params[0][0] == '#')
	{
		idx = this->isChannelExist(params[0]);
		if (idx == -1)
			return (this->sendToClient(client, "401 " + params[0] + " :No such nick/channel"), 1);
		this->sendMsgToChannel(client, this->chanPool[idx]->getMembers(), message);
	}
	else
	{
		idx = this->findUser(params[0], clients);
		if (idx == -1)
			return ( this->sendToClient(client, "401  :No such nick/channel"), 1);
		send(clients[idx]->getClientfd(), message.c_str(), message.length(), 0);
	}
	return 0;
}

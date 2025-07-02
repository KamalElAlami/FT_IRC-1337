#include "../../includes/Server.hpp"

int	Server::handlePingPong(Client* client, const std::vector<std::string>& params)
{
	std::string parameter;
	if (params.size() > 1)
		parameter = params[1];
	else
		parameter = "ircserv";
	std::string FullMsg = ": PONG :" + parameter + "\r\n";
	send(client->getClientfd(), FullMsg.c_str(), FullMsg.length(), 0);
	return 0;
}

/*
PRIVMSG kamal : kamal
*/
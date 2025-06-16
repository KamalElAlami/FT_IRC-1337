#include "../../includes/Server.hpp"

int Server::handleCap(Client* client, const std::vector<std::string>& params)
{
	if (params.empty())
		return (this->sendToClient(client, "461 CAP :Not enough parameters"), 1);
		
		std::string msg = "CAP * LS :\r\n";
		send(client->getClientfd(), msg.c_str(), msg.length(), 0);
	
	return 0;
}

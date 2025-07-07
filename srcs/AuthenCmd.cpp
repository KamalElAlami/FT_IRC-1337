#include "../includes/Server.hpp"

void	Server::ParseCommand(Client* client, std::string const & line)
{
	size_t						CmdPos = 0;
	size_t						ParamStart = 0;
	std::string					Command;
	std::string					middle;
	std::vector <std::string>	params;


	CmdPos = line.find(' ');
	if (CmdPos == std::string::npos)
		Command = line;
	else
		Command = line.substr(0, CmdPos);
	for (size_t i = 0; i < Command.length(); i++)
		Command[i] = toupper(Command[i]);
	if (CmdPos != std::string::npos)
	{
		ParamStart = CmdPos + 1;
		size_t triStart = line.find(" :", ParamStart);
		if (triStart != std::string::npos)
		{
			middle = line.substr(ParamStart, triStart - ParamStart);
			params = this->splitBySpaces(middle);
			params.push_back(line.substr(triStart + 2));
		}
		else
		{
			middle = line.substr(ParamStart);
			params = this->splitBySpaces(middle);
		}
	}
	if (Command == "PASS")
		this->handlePass(client, params);
	else if (Command == "CAP")
		this->handleCap(client, params);
	else if (Command == "NICK")
		this->handleNick(client, params);
	else if (Command == "USER")
		this->handleUser(client, params);
	else if (Command == "PING")
		this->handlePingPong(client, params);
	else if (Command == "JOIN")
		this->handleJoin(client, params);
	else if (Command == "PRIVMSG")
		this->handlePrivMsg(client, params);
	else if (Command == "PART")
		this->handlePart(client, params);
	else if (Command == "SBIKSLA")
		this->handleSbiksla(client, params);
	else if (Command == "MODE")
		this->handleMode(client, params);	
	else if (Command == "TOPIC")
		this->handleTopic(client, params);
	else if(Command == "INVITE")
		this->handleInvite(client, params);
	else if (Command == "KICK")
		this->handleKick(client, params);
	else if (Command != "WHO")
		this->sendToClient(client, "421 " + Command + " :Unknown command");
		// else if (Command == "QUIT")
		// 	this->handelQuit(client, params);
}

void Server::checkRegistration(Client* client)
{
	if (client->getRemoveClient() == true)
		this->removeClient(client->getClientfd());
	else if (!client->getUserName().empty() && !client->getNickName().empty() && !client->getPassword().empty()
		&& client->getPassword() == this->Password && client->getRegistered() == false)
	{
		client->setRegistered(true);
		this->sendToClient(client, "****************************************************");
		this->sendToClient(client, "001 " + client->getNickName() + " :Welcome to the Internet Relay Network " + 
			client->getNickName() + "!" + client->getUserName() + "@" + 
			client->getHostName());
		this->sendToClient(client, "002 " + client->getNickName() + 
			" :Your host is ircserv, running version 1.0");
		this->sendToClient(client, "003 " + client->getNickName() + 
			" :This server was created just now");
		this->sendToClient(client, "004 " + client->getNickName() + 
			" :ircserv v1.0");
			this->sendToClient(client, "****************************************************");
		std::cout << "Client " << client->getClientfd() << " is now registered as " << client->getNickName() << std::endl;
	}
}

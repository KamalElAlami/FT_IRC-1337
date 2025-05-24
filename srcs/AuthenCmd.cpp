#include "../includes/Server.hpp"

void	Server::ParseCommand(Client* client, std::string const & line)
{
	size_t						CmdPos = 0;
	size_t						ParamStart = 0;
	std::string					Command;
	std::string					middle;
	std::vector <std::string>	params;
	
	(void)client;
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
	// else if (Command == "TOPIC")
	// 	this->handleTopic(client, params);

}

int Server::handlePass(Client* client, const std::vector<std::string>& params)
{
	if (params.empty())
		return (this->removeClient(client->Clientfd), 
		this->sendToClient(client, "461 :Not enough parameters"), 1);
	if (client->registered)
		return (this->removeClient(client->Clientfd), 
		this->sendToClient(client, "462 :You may not reregister"), 1);
	if (params[0] != this->Password)
		return (this->removeClient(client->Clientfd), 
		this->sendToClient(client, "464 :Password incorrect"), 1);
	return (client->password = params[0], 0);
}

int		Server::handleNick(Client* client, const std::vector<std::string>& params)
{
	bool valid = true;
	std::string nickname = params[0];

	if(params.empty())
		return (this->removeClient(client->Clientfd), 
		this->sendToClient(client, "431 :No nickname given"), 1);
	if (nickname.empty() || nickname.length() > 10)
		valid = false;
	for (size_t i = 0; i < nickname.length() && valid == true; i++)
		if (!isalnum(nickname[i]) && nickname[i] != '-')
			valid = false;
	if (valid == false)
		return (this->removeClient(client->Clientfd), 
		this->sendToClient(client, "432 :[" + nickname + "] Erroneous nickname"), 1);
	for (size_t i = 0; i < this->clients.size(); i++)
	{
		if (this->clients[i] != client && this->clients[i]->nickName == nickname)
			return (this->removeClient(client->Clientfd), 
			this->sendToClient(client,
			"433 :[" + nickname + "] Nickname is already in use") ,1);
	}
	return (client->nickName = nickname, 0);
}

int	Server::handleUser(Client* client, const std::vector<std::string>& params)
{
	if(params.size() < 4)
		return (this->removeClient(client->Clientfd), 
		this->sendToClient(client, "461 :Not enough parameters"), 1);
	if (client->registered)
		return (this->removeClient(client->Clientfd), 
		this->sendToClient(client, "462 :You may not reregister"), 1);
	client->userName = params[0];
	client->hostName = params[1];
	client->realName = params[2];
	this->checkRegistration(client);
	return (0);
}

int	Server::handlePingPong(Client* client, const std::vector<std::string>& params)
{
	std::string parameter;
	if (params.size() > 1)
		parameter = params[1];
	else
		parameter = "ircserv";
	std::string FullMsg = ":ircserv PONG :" + parameter + "\r\n";
	send(client->Clientfd, FullMsg.c_str(), FullMsg.length(), 0);
	return 0;
}

int Server::handlePrivMsg(Client* client, const std::vector<std::string>& params)
{
	std::string message;
	int idx;

	if (params.size() != 2)
		return ( this->sendToClient(client, "461 :Not enough parameters"), 1);
	message = ":" + client->nickName + "!" + client->userName + "@localhost PRIVMSG " + params[0] + " :" + params[1] + "\r\n";
	if (params[0][0] == '#')
	{
		idx = this->findChannel(params[0]);
		if (idx == -1)
			return ( this->sendToClient(client, "401  :No such nick/channel"), 1);
		this->broadcastInChannel(this->chanPool[idx]->getMembers(), message);
	}
	else
	{
		idx = this->findUser(params[0]);
		if (idx == -1)
			return ( this->sendToClient(client, "401  :No such nick/channel"), 1);
		std::cout << message << std::endl;
		send(clients[idx]->Clientfd, message.c_str(), message.length(), 0);
	}
	return 0;
}

void Server::sendToClient(Client* client, const std::string& message)
{
	std::string fullMessage = ":ircserv " + message + "\r\n";
	send(client->Clientfd, fullMessage.c_str(), fullMessage.length(), 0);
}

void Server::checkRegistration(Client* client)
{
	if (!client->userName.empty() && !client->nickName.empty() && !client->password.empty()
		&& client->password == this->Password && client->registered == false)
	{
		client->registered = true;
		this->sendToClient(client, "****************************************************");
		this->sendToClient(client, "001 " + client->nickName + 
			" :Welcome to the Internet Relay Network " + 
			client->nickName + "!" + client->userName + "@" + 
			client->hostName);
		this->sendToClient(client, "002 " + client->nickName + 
			" :Your host is ircserv, running version 1.0");
		this->sendToClient(client, "003 " + client->nickName + 
			" :This server was created just now");
		this->sendToClient(client, "004 " + client->nickName + 
			" :ircserv 1.0 o o");
			this->sendToClient(client, "****************************************************");
		std::cout << "Client " << client->Clientfd << " is now registered as " << client->nickName << std::endl;
	}
}

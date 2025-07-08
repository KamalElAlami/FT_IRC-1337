#include "../../includes/Server.hpp"

int containsDangerousChars(const std::string& prompt)
{
    for (size_t i = 0; i < prompt.length(); i++)
    {
        char c = prompt[i];
        if (c == '\'' || c == '"' || c == '\\' || c == ';' || 
            c == '|' || c == '&' || c == '$' || c == '`' ||
            c == '(' || c == ')' || c == '<' || c == '>')
            return (1);
    }
    return (0);
}

int		Server::handleSbiksla(Client* client, const std::vector<std::string>& params)
{
    AiAgent a;
    int idx;
    std::string message;

    if (params.empty() || params.size() != 2)
        return (this->sendToClient(client, "461 :Not enough parameters"), 1);
    if (containsDangerousChars(params[1]))
            return (sendToClient(client, "NOTICE " + client->getNickName() + " :Try rephrasing without special characters like (/;|&$\"\'\\{})"), 1);
	
    Client *bot = getBotInstance();
    a.setApi("AIzaSyD6izOtFIw6IvaAAKdI7DVy6eARBpefLbY");
    std::string response = a.startAgent(params[1]);
    message = ":" + bot->getNickName() + "!" + bot->getUserName() + "@localhost PRIVMSG " + params[0] + " :" + response + "\r\n";
    if (params[0][0] == '#')
	{
        std::string clientMsg = ":" + client->getNickName() + "!" + client->getUserName() + "@" + client->getHostName() + " PRIVMSG " + params[0] + " :" + params[1];
		idx = this->isChannelExist(params[0]);
		if (idx == -1)
			return (this->sendToClient(client, "401 " + params[0] + " :No such nick/channel"), 1);
        broadcastInChannel(chanPool[idx]->getMembers(), clientMsg);
		this->sendMsgToChannel(bot, this->chanPool[idx]->getMembers(), message);
	}
	else if (params[0].compare("prvmsg") == 0)
		send(client->getClientfd(), message.c_str(), message.length(), 0);
    return (0);
}

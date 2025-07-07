#include "../includes/Server.hpp"
#include "../includes/Channels.hpp"

bool Server::signals = false;

int Server::isChannelExist(std::string chanName)
{
    if (chanPool.empty())
        return (-1);
    if (chanName[0] != '#')
        chanName = "#" + chanName;
    for (size_t i = 0; i < chanPool.size(); i++)
    {
        if (chanName == chanPool[i]->getName())
            return (i);
    }
    return (-1);
}

void Server::broadcastInChannel(std::vector <Client *> members, std::string message)
{
    if (message.length() < 2 || message.substr(message.length() - 2) != "\r\n")
        message += "\r\n";
    for (size_t i = 0; i < members.size(); i++)
        send(members[i]->getClientfd(), message.c_str(), message.length(), 0);
}

void Server::broadcastInChannel(std::vector <Client *> members, std::string message, const Client &client)
{
    if (message.length() < 2 || message.substr(message.length() - 2) != "\r\n")
        message += "\r\n";
    for (size_t i = 0; i < members.size(); i++){
        if (client.getClientfd() != members[i]->getClientfd())
            send(members[i]->getClientfd(), message.c_str(), message.length(), 0);
    }
}


void Server::sendMsgToChannel(Client* client, std::vector <Client *> members, std::string message)
{
    if (message.length() < 2 || message.substr(message.length() - 2) != "\r\n")
        message += "\r\n";
    for (size_t i = 0; i < members.size(); i++)
    {
        if (client->getClientfd() == members[i]->getClientfd())
            continue;
        send(members[i]->getClientfd(), message.c_str(), message.length(), 0);
    }
}

int Server::findUser(std::string name, std::vector <Client*> cli)
{
    for (size_t i = 0; i < cli.size(); i++)
        if (name == cli[i]->getNickName())
            return(i);
    return (-1);
}

std::vector<std::string> ft_split(std::string str, char c)
{
    size_t start = 0;
    size_t end = 0;
    std::vector<std::string> words;

    while ((end = str.find(c, start)) != std::string::npos) {
        words.push_back(str.substr(start, end - start));
        start = end + 1;
    }
    words.push_back(str.substr(start));
    return (words);
}

void Server::sendToClient(Client* client, const std::string& message)
{
    std::string fullMessage = message;
    if (fullMessage.length() < 2 || fullMessage.substr(fullMessage.length() - 2) != "\r\n")
    {
        fullMessage += "\r\n";
    }
	send(client->getClientfd(), fullMessage.c_str(), fullMessage.length(), 0);
}

void Server::SigHandler(int value)
{
	(void) value;
	std::cout << "\n*------------------* Ircserv has ben closed *------------------*" << std::endl;
	Server::signals = true;
	//Server::ClearAll();
	//for (size_t i = 0; i < clients.size(); i++)
	//{
	//	/* code */
	//}
	
	//handelQuit(); // Removed 'this->' since it's a non-static member function
	//ClearAll(); // Now this can be called without 'this->'
	//exit (0);
}

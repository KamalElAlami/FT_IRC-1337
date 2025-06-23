#include "../includes/Server.hpp"
#include "../includes/Channels.hpp"

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
        std::cout << members[i]->getNickName() << std::endl;
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

size_t numberOfParameterizedArgs(std::string arg)
{
    size_t count = 0;
    for (int i = 0; arg.c_str()[i]; i++)
        if ((arg.c_str()[i] == 'o') || (arg.c_str()[i] == 'k') || (arg.c_str()[i] == 'l'))
            count += 1;
    return (count);
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


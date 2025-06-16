#include "../includes/Server.hpp"
#include "../includes/Channels.hpp"

int Server::isChannelExist(std::string chanName)
{
    if (chanPool.empty())
        return (-1);
    for (size_t i = 0; i < chanPool.size(); i++)
        if (chanName == chanPool[i]->getName())
            return (i);
    return (-1);
}

void Server::broadcastInChannel(std::vector <Client *> members, std::string message)
{
    if (message.length() < 2 || message.substr(message.length() - 2) != "\r\n")
        message += "\r\n";
//    std::cout << message << std::endl;
//    for (size_t i = 0; i < members.size(); i++)
//        {
//            std::cout << members[i]->Clientfd << " ";
//        }
     //   std::cout << std::endl;
    for (size_t i = 0; i < members.size(); i++)
    {
     //   std::cout << "client fd : " << members[i]->Clientfd << std::endl;
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
	std::string fullMessage = message + "\r\n";
	send(client->getClientfd(), fullMessage.c_str(), fullMessage.length(), 0);
}

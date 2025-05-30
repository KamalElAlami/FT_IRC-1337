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
    std::cout << message << std::endl;
    for (size_t i = 0; i < members.size(); i++)
        {
            std::cout << members[i]->Clientfd << " ";
        }
        std::cout << std::endl;
    for (size_t i = 0; i < members.size(); i++)
    {
        std::cout << "client fd : " << members[i]->Clientfd << std::endl;
        send(members[i]->Clientfd, message.c_str(), message.length(), 0);
    }
}


void Server::sendMsgToChannel(Client* client, std::vector <Client *> members, std::string message)
{
    if (message.length() < 2 || message.substr(message.length() - 2) != "\r\n")
        message += "\r\n";
    for (size_t i = 0; i < members.size(); i++)
    {
        if (client->Clientfd == members[i]->Clientfd)
            continue;
        send(members[i]->Clientfd, message.c_str(), message.length(), 0);
    }
}

int Server::findUser(std::string name, std::vector <Client*> cli)
{
    for (size_t i = 0; i < cli.size(); i++)
        if (name == cli[i]->nickName)
            return(i);
    return (-1);
}
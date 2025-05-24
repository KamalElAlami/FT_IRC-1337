#include "../includes/Server.hpp"
#include "../includes/Channels.hpp"

bool Server::isChannelExist(std::string chanName)
{
    // if (chanPool.empty())
    //     return (false);
    for (size_t i = 0; i < chanPool.size(); i++)
        if (chanName == chanPool[i]->getName())
            return (true);
    return (false);
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

int Server::findChannel(std::string name)
{
    for (size_t i = 0; i < this->chanPool.size(); i++)
        if (name == chanPool[i]->getName())
            return(i);
    return (-1);
}
int Server::findUser(std::string name)
{
    for (size_t i = 0; i < this->clients.size(); i++)
        if (name == clients[i]->nickName)
            return(i);
    return (-1);
}
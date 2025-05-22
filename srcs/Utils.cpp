#include "../includes/Server.hpp"
#include "../includes/Channels.hpp"

int Server::isChannelExist(std::string chanName)
{
    for (size_t i = 0; i <= chanPool.size(); i++)
        if (chanName == chanPool[i]->getName())
            return (true);
    return (false);
}

void Server::broadcastInChannel(std::vector <Client *> members, std::string message)
{
    for (size_t i = 0; i <= members.size(); i++)
        send(members[i]->Clientfd, message.c_str(), message.length(), 0);
}
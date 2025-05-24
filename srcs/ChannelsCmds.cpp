#include "../includes/Server.hpp"
#include "../includes/Channels.hpp"

int Server::handleJoin(Client* client, const std::vector<std::string>& params)
{
    std::string announce;
    if (params.empty())
        return (this->sendToClient(client, "461 :Not enough parameters"), 1);
    if (params[0][0] != '#')
        return (this->sendToClient(client, "403 :Forbidden channel name"), 1);
    if (!this->isChannelExist(params[0]))
    {
        Channel *tmp = new Channel(params[0]);
        this->chanPool.push_back(tmp);
        tmp->addToContainer(client, tmp->getOperators());
    }
    int chanIndex = this->chanPool.size() - 1;
    this->chanPool[chanIndex]->getMembers().push_back(client);
    announce = ":" + client->nickName + "!" + client->userName + "@localhost " + "JOIN " + params[0];
    this->broadcastInChannel(this->chanPool[chanIndex]->getMembers(), announce);
    return (0);
}

// int Server::handleTopic(Client* client, const std::vector<std::string>& params)
// {

// }
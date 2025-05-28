#include "../includes/Server.hpp"
#include "../includes/Channels.hpp"

int Server::handleJoin(Client* client, const std::vector<std::string>& params)
{
    std::string announce;
    int chanIndex;
    if (params.empty())
        return (this->sendToClient(client, "461 :Not enough parameters"), 1);
    if (params[0][0] != '#')
        return (this->sendToClient(client, "403 :Forbidden channel name"), 1);
    chanIndex = this->isChannelExist(params[0]);
    if (chanIndex == -1)
    {
        Channel *tmp = new Channel(params[0]);
        this->chanPool.push_back(tmp);
        tmp->addToContainer(client, tmp->getOperators());
        chanIndex = this->chanPool.size() - 1;
    }
    this->chanPool[chanIndex]->getMembers().push_back(client);
    announce = ":" + client->nickName + "!" + client->userName + "@localhost " + "JOIN " + params[0];
    this->broadcastInChannel(this->chanPool[chanIndex]->getMembers(), announce);
    return (0);
}

int Server::handlePart(Client* client, const std::vector<std::string>& params)
{
    if (params.empty())
        return (this->sendToClient(client, "461 :Not enough parameters"), 1);
    int chanIndex = this->isChannelExist(params[0]);
    if (chanIndex == -1)
        return (this->sendToClient(client, "403 :No such channel"), 1);
    int userIndex = this->findUser(client->nickName, this->chanPool[chanIndex]->getMembers());
    if (userIndex == -1)
        return (this->sendToClient(client, "442 :You're not on that channel"), 1);
    std::string reason = (params.size() > 1) ? params[1] : "See Later Guys";
    std::string message = ":" + client->nickName + "!" + client->userName + "@localhost PART " + params[0] + " :" + reason + "\r\n";
    this->sendMsgToChannel(client, this->chanPool[chanIndex]->getMembers(), message);
    this->chanPool[chanIndex]->deleteFromContainer(this->chanPool[chanIndex]->getMembers()[userIndex], this->chanPool[chanIndex]->getMembers());
    int opIndex = this->findUser(client->nickName, this->chanPool[chanIndex]->getOperators());
    if (this->chanPool[chanIndex]->getOperators().size() == 1 && opIndex != -1)
    {
        if (this->chanPool[chanIndex]->getMembers().size() > 1)
            this->chanPool[chanIndex]->addToContainer(this->chanPool[chanIndex]->getMembers()[0], this->chanPool[chanIndex]->getOperators());
        else
        {
            delete this->chanPool[chanIndex];
            this->chanPool.erase(this->chanPool.begin() + chanIndex);
        }
    }
    else if (opIndex != -1)
        this->chanPool[chanIndex]->deleteFromContainer(this->chanPool[chanIndex]->getOperators()[opIndex], this->chanPool[chanIndex]->getOperators());
    
    return (0);
}

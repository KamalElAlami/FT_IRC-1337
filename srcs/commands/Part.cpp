#include "../../includes/Server.hpp"

int Server::partUserByUser(Client* client, std::string channel, const std::vector<std::string>& params)
{
    int chanIndex = this->isChannelExist(channel);
    if (chanIndex == -1)
        return (this->sendToClient(client, "403 : No such channel"), 1);
    int userIndex = this->findUser(client->getNickName(), this->chanPool[chanIndex]->getMembers());
    if (userIndex == -1)
        return (this->sendToClient(client, "442 : You're not on that channel"), 1);
    std::string reason = (params.size() == 2) ? params[1] : "See Later Guys";
    std::string message = ":" + client->getNickName() + "!" + client->getUserName() + "@localhost PART " + channel + " :" + reason + "\r\n";
    this->sendMsgToChannel(client, this->chanPool[chanIndex]->getMembers(), message);
    this->sendToClient(client, message);
    this->chanPool[chanIndex]->deleteFromContainer(this->chanPool[chanIndex]->getMembers()[userIndex], this->chanPool[chanIndex]->getMembers());
    int opIndex = this->findUser(client->getNickName(), this->chanPool[chanIndex]->getOperators());
    if (this->chanPool[chanIndex]->getOperators().size() == 1 && opIndex != -1)
    {
        if (this->chanPool[chanIndex]->getMembers().size() >= 1){
            this->chanPool[chanIndex]->addToContainer(this->chanPool[chanIndex]->getMembers()[0], this->chanPool[chanIndex]->getOperators());
            std::string announce = ":" + client->getNickName() + "!" + "localhost MODE "
            + this->chanPool[chanIndex]->getName() + " +o " + this->chanPool[chanIndex]->getMembers()[0]->getNickName();
            broadcastInChannel(this->chanPool[chanIndex]->getMembers(), announce);
        }
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

int Server::handlePart(Client* client, const std::vector<std::string>& params)
{
    if (params.empty())
        return (this->sendToClient(client, "461 : Not enough parameters"), 1);
    if (params[0].find(",") != std::string::npos)
    {
        std::vector<std::string> canals = ft_split(params[0], ',');
        for (size_t i = 0; i < canals.size(); i++)
            partUserByUser(client, canals[i], params);
    }
    else
        partUserByUser(client, params[0], params);


    
    return (0);
}


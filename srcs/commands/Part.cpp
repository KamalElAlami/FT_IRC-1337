#include "../../includes/Server.hpp"

int Server::handlePart(Client* client, const std::vector<std::string>& params)
{
    if (params.empty())
        return (this->sendToClient(client, "461 : Not enough parameters"), 1);
    int chanIndex = this->isChannelExist(params[0]);
    if (chanIndex == -1)
        return (this->sendToClient(client, "403 : No such channel"), 1);
    std::cout << "we are in handlePart" << std::endl;
    int userIndex = this->findUser(client->getNickName(), this->chanPool[chanIndex]->getMembers());
    if (userIndex == -1)
        return (this->sendToClient(client, "442 : You're not on that channel"), 1);
    std::string reason = (params.size() > 1) ? params[1] : "See Later Guys";
    std::string message = ":" + client->getNickName() + "!" + client->getUserName() + "@localhost PART " + params[0] + " :" + reason + "\r\n";
    this->sendMsgToChannel(client, this->chanPool[chanIndex]->getMembers(), message);
    this->sendToClient(client, message);
    this->chanPool[chanIndex]->deleteFromContainer(this->chanPool[chanIndex]->getMembers()[userIndex], this->chanPool[chanIndex]->getMembers());
    int opIndex = this->findUser(client->getNickName(), this->chanPool[chanIndex]->getOperators());
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


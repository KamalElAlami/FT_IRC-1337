#include "../../includes/Server.hpp"

void Server::sendNamesRpl(Client* client, std::string channelName, int chanIndex)
{
    std::string namesList = "353 " + client->getNickName() + " = " + channelName + " :";
    
    for (size_t i = 0; i < this->chanPool[chanIndex]->getMembers().size(); i++)
    {
        Client* member = this->chanPool[chanIndex]->getMembers()[i];
        
        if (this->findUser(member->getNickName(), this->chanPool[chanIndex]->getOperators()) != -1)
            namesList += "@";
        namesList += member->getNickName();
        if (i < this->chanPool[chanIndex]->getMembers().size() - 1)
            namesList += " ";
    }
    
    sendToClient(client, namesList);
    sendToClient(client, "366 " + client->getNickName() + " " + channelName + " :End of /NAMES list");
}

void    Server::createChannel(Client* client , std::string channelName, const std::vector<std::string>& params)
{
    std::string announce;
    int chanIndex;
    std::cout << channelName << channelName.size() << std::endl;
    announce = ":" + client->getNickName() + "!" + client->getUserName() + "@localhost " + "JOIN " + channelName;
    chanIndex = this->isChannelExist(channelName);
    if (chanIndex == -1)
    {
        Channel *tmp = new Channel(channelName);
        this->chanPool.push_back(tmp);
        tmp->addToContainer(client, tmp->getOperators());
        chanIndex = this->chanPool.size() - 1;

        this->chanPool[chanIndex]->getMembers().push_back(client);
        this->broadcastInChannel(this->chanPool[chanIndex]->getMembers(), announce);
        return (sendNamesRpl(client, channelName, chanIndex));
    }
    if (this->chanPool[chanIndex]->getMemberLimit() != -1 && this->chanPool[chanIndex]->getMemberLimit() < (int)(this->chanPool[chanIndex]->getMembers().size() + 1))//handel an error 
        return (sendToClient(client, "471 " + channelName + " :Cannot join channel (+l)"));
    if (this->chanPool[chanIndex]->getInviteOnly() && !client->isInvited(channelName))
        return (sendToClient(client, "473 " + channelName + " :Cannot join channel (+i)"));
    if (this->chanPool[chanIndex]->getEnabledPass())
    {
        if (params.size() < 2 || this->chanPool[chanIndex]->getPassword() != params[1])
            return (sendToClient(client, "475 " + channelName + " :Cannot join channel (+k)"));
    }
    if (this->chanPool[chanIndex]->getInviteOnly())
        client->setInvite(channelName, false);
    this->chanPool[chanIndex]->getMembers().push_back(client);
    this->broadcastInChannel(this->chanPool[chanIndex]->getMembers(), announce);
    if (!this->chanPool[chanIndex]->getTopic().empty())
        sendToClient(client, "332 "+ client->getNickName() + " " + channelName + " :" + this->chanPool[chanIndex]->getTopic());
    sendNamesRpl(client, channelName, chanIndex);
}

void    Server::createChannel(Client* client , std::string channelName, const std::vector<std::string>& params, std::string pass)
{
    std::string announce;
    int chanIndex;

    announce = ":" + client->getNickName() + "!" + client->getUserName() + "@localhost " + "JOIN " + channelName;
    chanIndex = this->isChannelExist(channelName);
    if (chanIndex == -1)
    {
        Channel *tmp = new Channel(channelName);
        this->chanPool.push_back(tmp);
        tmp->addToContainer(client, tmp->getOperators());
        chanIndex = this->chanPool.size() - 1;

        this->chanPool[chanIndex]->getMembers().push_back(client);
        return (this->broadcastInChannel(this->chanPool[chanIndex]->getMembers(), announce));
    }
    if (this->chanPool[chanIndex]->getMemberLimit() != -1 && this->chanPool[chanIndex]->getMemberLimit() <= (int)(this->chanPool[chanIndex]->getMembers().size() + 1))
        return (sendToClient(client, "471 " + channelName + " :Cannot join channel (+l)"));
    if (this->chanPool[chanIndex]->getInviteOnly() && !client->isInvited(channelName))
        return (sendToClient(client, "473 " + channelName + " :Cannot join channel (+i)"));
    if (this->chanPool[chanIndex]->getEnabledPass())
    {
        if (params.size() < 2 || this->chanPool[chanIndex]->getPassword() != pass)
            return (sendToClient(client, "475 " + channelName + " :Cannot join channel (+k)"));
    }
    if (this->chanPool[chanIndex]->getInviteOnly())
        client->setInvite(channelName, false);
    this->chanPool[chanIndex]->getMembers().push_back(client);
    this->broadcastInChannel(this->chanPool[chanIndex]->getMembers(), announce);
    sendNamesRpl(client, channelName, chanIndex);
}

int Server::handleJoin(Client* client, const std::vector<std::string>& params)
{

    if (params.empty())
        return (this->sendToClient(client, "461 :Not enough parameters"), 1);
    if (params[0][0] != '#')
        return (this->sendToClient(client, "403 :Forbidden channel name"), 1);
    if (params[0].find(",") != std::string::npos)
    {
        std::vector<std::string> canals = ft_split(params[0], ',');
        for (size_t i = 0; i < canals.size(); i++)
        {
            if (params.size() == 1)
                createChannel(client, canals[i], params);
            else
            {
                std::vector<std::string> pass = ft_split(params[1], ','); // split multiple passwords pass1,pass2,pass3
                createChannel(client, canals[i], params, pass[i]);
            }

        }
    }
    else
        createChannel(client, params[0], params);
    return (0);
}
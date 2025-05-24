#include "../includes/Server.hpp"
#include "../includes/Channels.hpp"


Channel::Channel(std::string n) : name(n)
{
}

std::string& Channel::getName(void)
{
    return (name);
}
std::string& Channel::getTopic(void)
{
    return (topic);
}

std::vector <Client *>& Channel::getMembers(void)
{
    return (members);
}

std::vector <Client *>& Channel::getOperators(void)
{
    return (ops);
}

void Channel::addToContainer(Client* client, std::vector <Client *> Container)
{
    Container.push_back(client); //  maybe needs protection
}

int Channel::deleteFromContainer(Client* client, std::vector <Client *> Container)
{
    for (size_t i = 0; i <= Container.size(); i++)
    {
        if (client->Clientfd == Container[i]->Clientfd)
        {
            Container.erase(Container.begin() + i);
            return (1);
        }

    }
    return (0);
}
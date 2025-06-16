#include "../includes/Server.hpp"
#include "../includes/Channels.hpp"


Channel::Channel(std::string n) : name(n)
{
    inviteOnly = false;
    restrictedTopic = false;
    enabledPass = false;
    memberLimit = -1;
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

bool& Channel::getInviteOnly(void)
{
    return (inviteOnly);
}
bool& Channel::getRestrictedTopic(void)
{
    return (restrictedTopic);
}
bool& Channel::getEnabledPass(void)
{
    return (enabledPass);
}
int&  Channel::getMemberLimit(void)
{
    return (memberLimit);
}
std::string& Channel::getPassword(void)
{
    return (Password);
}

void Channel::addToContainer(Client* client, std::vector <Client *>& Container)
{
    Container.push_back(client); //  maybe needs protection
}

int Channel::deleteFromContainer(Client* client, std::vector <Client *>& Container)
{
    for (size_t i = 0; i < Container.size(); i++)
    {
        if (client->getClientfd() == Container[i]->getClientfd())
        {
            Container.erase(Container.begin() + i);
            return (1);
        }

    }
    return (0);
}
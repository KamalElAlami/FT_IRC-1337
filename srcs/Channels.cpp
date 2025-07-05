#include "../includes/Server.hpp"
#include "../includes/Channels.hpp"


Channel::Channel(std::string n) : name(n)
{
    topic = "";
    inviteOnly = false;
    restrictedTopic = true;
    enabledPass = false;
    memberLimit = -1;
    creationTime = std::time(0);
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
    Container.push_back(client);
}

int Channel::deleteFromContainer(Client* client, std::vector <Client *>& Container)
{
    for (size_t i = 0; i < Container.size(); i++)
    {
        if (client && client->getClientfd() == Container[i]->getClientfd())
        {
            Container.erase(Container.begin() + i);
            return (1);
        }

    }
    return (0);
}

void Channel::setEnabledPass(bool stt)
{
    enabledPass = stt;
}

bool Channel::hasUser(int _client_fd){
    for (size_t i = 0; i < members.size(); i++){
        if (_client_fd == members[i]->getClientfd())
            return true;
    }
    return false;
}

std::string Channel::getTopicSetBy()const{
    return this->topicSetBy;
}

time_t Channel::getTopicSetAt() const{
    return this->topicSetAt;
}

time_t& Channel::getCreationTime() 
{
    return (creationTime);
}

bool Channel::isOperator(int client_fd)const {
    for (size_t i = 0; i < ops.size(); i++){
        if (client_fd == ops[i]->getClientfd())
            return true;
    }
    return false;
}

void Channel::setTopic(std::string _topic){
    this->topic = _topic;
}

void Channel::setTopicSetBy(std::string _client){
    this->topicSetBy = _client;
}

void Channel::setTopicSetAt(time_t _time){
    this->topicSetAt = _time;
}

void Channel::setCreationTime(time_t tt)
{
    creationTime = tt;
}
void Channel::setInviteOnly(bool status){
    this->inviteOnly = status;
}

void Channel::setRestrictedTopic(bool status){
    this->restrictedTopic = status;
}

void Channel::setPassword(std::string key){
    this->Password = key;
}

void Channel::setMemberLimit(int n){
    this->memberLimit = n;
}
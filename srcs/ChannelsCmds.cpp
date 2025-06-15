#include "../includes/Server.hpp"
#include "../includes/Channels.hpp"

static std::vector<std::string> ft_split(std::string str, char c)
{
    size_t start = 0;
    size_t end = 0;
    std::vector<std::string> words;

    while ((end = str.find(c, start)) != std::string::npos) {
        words.push_back(str.substr(start, end - start));
        start = end + 1;
    }
    words.push_back(str.substr(start));
    return (words);
}

void    Server::createChannel(Client* client , std::string channelName)
{
    std::string announce;
    int chanIndex;

    chanIndex = this->isChannelExist(channelName);
    if (chanIndex == -1)
    {
        Channel *tmp = new Channel(channelName);
        this->chanPool.push_back(tmp);
        tmp->addToContainer(client, tmp->getOperators());
        chanIndex = this->chanPool.size() - 1;
    }
    this->chanPool[chanIndex]->getMembers().push_back(client);
    announce = ":" + client->nickName + "!" + client->userName + "@localhost " + "JOIN " + channelName;
    this->broadcastInChannel(this->chanPool[chanIndex]->getMembers(), announce);
}

int Server::handleJoin(Client* client, const std::vector<std::string>& params)
{

    if (params.empty())
        return (this->sendToClient(client, "461 :Not enough parameters"), 1);
    if (params[0][0] != '#')
        return (this->sendToClient(client, "403 :Forbidden channel name"), 1);
    if (params[0].find(","))
    {
        std::vector<std::string> canals = ft_split(params[0], ',');
        for (size_t i = 0; i < canals.size(); i++)
            createChannel(client, canals[i]);
    }
    else
        createChannel(client, params[0]);
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

int		Server::handleMode(Client* client, const std::vector<std::string>& params)
{
    if (params.empty())
        return (this->sendToClient(client, "461 :Not enough parameters"), 1);
    if (numberOfParameterizedArgs(params[1]) > (params.size() - 2))
        return (this->sendToClient(client, "461 :Not enough parameters"), 1);
    int chanIndex = this->isChannelExist(params[0]);
    if (chanIndex == -1)
        return (this->sendToClient(client, "403 :No such channel"), 1);
    if (this->findUser(client->nickName, chanPool[chanIndex]->getOperators()) == -1)
        return (this->sendToClient(client, "482 " + params[0] + " :You're not channel operator"), 1);
    // if (params[1].size() > 2)
        // hundleJoinededArgs();
    // hundleShuffledArgs();

    return (0);
}


/// mode #channel +iok kamal lsdkjdf
/// mode #cjan +i 

// int		Server::handleTopic(Client* client, const std::vector<std::string>& params){
//     for (size_t i = 0; i < params.size(); i++)
//         std::cout << params[i] << std::endl;
//     if (params.empty())
//         return (sendToClient(client, "461: Not enough parameters"), 1);
//     std::string channelName = params[0];
//     Channel *_channel = findChannel(channelName);
//     if (!_channel)
//         return (sendToClient(client, "442" + channelName + " :No such channel"), 1);
//     if (!_channel->hasUser(client->getClientFd()))
//         return (sendToClient(client, "442" + channelName + " :You're not on that channel"), 1);
//     if (params.size() == 1 && _channel){
//         if (_channel->getTopic().empty())
//             return (sendToClient(client, "331" + channelName + " :No topic is set"), 1);
//         else{
//             sendToClient(client, "332" + channelName + " :" + _channel->getTopic());
//             sendToClient(client, "333" + channelName + " " + _channel->getTopicSetBy() + " " + std::to_string(_channel->getTopicSetAt()));
//         }
//     }
//     if (params.size() > 1){
//         if (_channel->getIsTopicProtected() && _channel->isOperator(client->getClientFd()))
//             return (sendToClient(client, "482" + channelName + " :You're not channel operator"), 1);
//             _channel->setTopic(params[1]);
//             _channel->setTopicSetBy(client->nickName);
//             _channel->setTopicSetAt(std::time(0));
//     }
//     std::string message = ":" + client->getPrefix() + " TOPIC " + channelName + ":" + params[1];
//     this->broadcastInChannel(_channel->getMembers(), message);
//     return 0;
// }


int		Server::handleInvite(Client *client, const std::vector<std::string> &params){
    (void)client;
    //  if (params.empty() || params.size() != 2){//usless
    //     std::cout << "commande empty\n";
    //      return (sendError(*client, "461", "INVITE :Not enough parameters"), 1);
    // }
    std::string channelName = params[1];
    std::cout << "channel name is: " + channelName << std::endl;
    Channel *_channel = findChannel(channelName);
    if (_channel == NULL)
        return (sendError(*client, "403", "No such channel"), 1);
    int new_clientFd = getclientfd(params[0]);
    if (new_clientFd == -1)
        return (sendError(*client, "482", "no user with this nickname"), 1);
    if (!_channel->hasUser(client->getClientFd()))
        return (sendError(*client, "442", "You're not on that channel"), 1);
    if (_channel->getInviteOnly() && !_channel->isOperator(client->Clientfd))
        return (sendError(*client, "482", "You're not an operator in that channel"), 1);
    if (_channel->hasUser(new_clientFd))
        return(sendError(*client, "443", "user already in channel"),1);
    Client *new_client = getClient(new_clientFd);
    _channel->addToContainer(new_client, _channel->getMembers());
    sendToClient(new_client, "341" + new_client->getNick() + channelName);
    std::string announce = ":" + client->nickName + "!" + client->userName + "@localhost " + "INVITE " + new_client->nickName + " " + channelName;
    this->broadcastInChannel(_channel->getMembers(), announce);
    return 0;
}

int 	Server::handleKick(Client* client, const std::vector<std::string>& params){
    for (size_t i = 0; i < params.size(); i++)// check input = i didn't understand this input
        std::cout << params[i] << std::endl;
    Channel *_channel = findChannel(params[0]);
    if (!_channel)
        return (sendError(*client, "403", "No such channel"), 1);
    int new_clientFd = getclientfd(params[1]);
    if (new_clientFd == -1)
        return (sendError(*client, "482", "no user with this nickname"), 1);
    if (!_channel->hasUser(client->getClientFd()))
        return (sendError(*client, "442", "You're not on that channel"), 1);
    if (!_channel->isOperator(client->Clientfd))
        return (sendError(*client, "482", "You're not an operator in that channel"), 1);
    Client *new_client = getClient(new_clientFd);
    if (_channel->deleteFromContainer(new_client, _channel->getMembers())){
        std::string comment = "";
        if (params.size() > 2 && params[2].empty())
            comment = params[2];
        else
            comment = "You have been kicked from the channel.";
        
        std::string announce = ":" + client->nickName + "!" + client->userName + "@localhost " + "KICK " + params[0] + " " +new_client->nickName;
        this->broadcastInChannel(_channel->getMembers(), announce);
    }
    else
        return(sendError(*client, "441", "user not in this channel"),1);
    return 0;
}


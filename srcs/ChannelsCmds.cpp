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
    announce = ":" + client->getNickName() + "!" + client->getUserName() + "@localhost " + "JOIN " + channelName;
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

int Server::handlePart(Client* client, const std::vector<std::string>& params)//remove from operator
{
   if (params.empty())
       return (this->sendToClient(client, "461 :Not enough parameters"), 1);

    std::stringstream ss(params[0]);
    std::string channelName;
    
    while(std::getline(ss, channelName, ',')){

        Channel *_channel = findChannel(channelName);
        
        if (_channel == NULL){
            sendError(client->getClientfd(), "403", channelName, "No such channel");
            continue;
        }
        
        if (!_channel->hasUser(client->getClientfd())){
            sendError(client->getClientfd(), "442", channelName,"You're not on that channel");
            continue;
        }
        
        std::string message = ":" + client->getNickName() + "!" + client->getUserName() + "@localhost"+ " PART " + channelName;
        if (params.size() > 1 && !params[1].empty())
           message += " :" + params[1];
        sendToClient(client, message);
        broadcastInChannel(_channel->getMembers(), message);
        _channel->deleteFromContainer(client, _channel->getMembers());
        if (_channel->getMembers().size() == 0) {
            std::vector<Channel*>::iterator it = std::find(chanPool.begin(), chanPool.end(), _channel);
            chanPool.erase(it);
        }
    }
   return (0);
}

Client* Server::createBot(void)
{
	Client* bot = new Client();
	bot->setNickName("Sbiksla");
	bot->setUserName("Sbiksla");
	bot->setRealName("Sbiksla");
	bot->setRegistered(true);
	bot->setClientfd(-1);
	bot->setAddress("localhost");
	this->clients.push_back(bot);
    return (bot);
}

int		Server::handleSbiksla(Client* client, const std::vector<std::string>& params)
{
    // potential leaks and maybe unprotected stuff 
    // usage : /SBIKSLA #channel_NAME :prompt
    // /SBIKSLA prvmsg :prompt for dm
    AiAgent a;
    int idx;
    std::string message;

    if (params.empty())
        return (this->sendToClient(client, "461 :Not enough parameters"), 1);
    Client *bot = createBot();
    a.setApi("AIzaSyD6izOtFIw6IvaAAKdI7DVy6eARBpefLbY");
    std::string response = a.startAgent(params[1]);
    message = ":" + bot->getNickName() + "!" + bot->getUserName() + "@localhost PRIVMSG " + params[0] + " :" + response + "\r\n";
    std::cout << "Response: " << message << std::endl;
    if (params[0][0] == '#')
	{
		idx = this->isChannelExist(params[0]);
		if (idx == -1)
			return (this->sendToClient(client, "401 " + params[0] + " :No such nick/channel"), 1);
		this->sendMsgToChannel(bot, this->chanPool[idx]->getMembers(), message);
	}
	else if (params[0].compare("prvmsg") == 0)
		send(client->getClientfd(), message.c_str(), message.length(), 0);
    return (0);
}

//added by soufiix
int		Server::handleTopic(Client* client, const std::vector<std::string>& params){
    if (params.empty())
        return (sendError(client->getClientfd(), "461", "TOPIC" ,"Not enough parameters"), 1);

    std::string channelName = params[0];
    Channel *_channel = findChannel(channelName);

    if (!_channel)
        return (sendError(client->getClientfd(), "403", channelName, "No such channel"), 1);

    if (!_channel->hasUser(client->getClientfd()))
        return (sendError(client->getClientfd(), "442", channelName,"You're not on that channel"), 1);

    if (params.size() == 1 && _channel){
        if (_channel->getTopic().empty())
            return (sendError(client->getClientfd(), "331", channelName, ":No topic is set"), 1);
        else{
            std::ostringstream oss;
            oss << _channel->getTopicSetAt();
            std::string TopicSetAt = oss.str();
            sendToClient(client, "332 "+ client->getNickName() + " " + channelName + " :" + _channel->getTopic());
            sendToClient(client, "333 " + channelName + " " + _channel->getTopicSetBy() + " " + TopicSetAt);
        }
    }

    if (params.size() > 1){
        if (_channel->getRestrictedTopic() && !_channel->isOperator(client->getClientfd()))
            return (sendError(client->getClientfd(), "482", channelName ,"You're not an operator in that channel"), 1);

        _channel->setTopic(params[1]);
        _channel->setTopicSetBy(client->getNickName());
        _channel->setTopicSetAt(std::time(0));
    }

    std::string message = ":" + client->getNickName() + "!" + client->getUserName() + "@localhost" + " TOPIC " + channelName + " :" + params[1];
    this->broadcastInChannel(_channel->getMembers(), message);
    return 0;
}


int		Server::handleInvite(Client *client, const std::vector<std::string> &params){
    if (params.empty() || params.size() != 2)
         return (sendError(client->getClientfd(), "461", "INVITE" ,"Not enough parameters"), 1);

    std::string channelName = params[1];
    Channel *_channel = findChannel(channelName);

    if (_channel == NULL)
        return (sendError(client->getClientfd(), "403", channelName, "No such channel"), 1);

    int new_clientFd = getclientfd(params[0]);

    if (new_clientFd == -1)
        return (sendError(client->getClientfd(), "401", params[0],"No such nick"), 1);

    Client *new_client = getClient(new_clientFd);

    if (!_channel->hasUser(client->getClientfd()))
        return (sendError(client->getClientfd(), "442", channelName,"You're not on that channel"), 1);

    if (_channel->getInviteOnly() && !_channel->isOperator(client->getClientfd()))
        return (sendError(client->getClientfd(), "482", channelName ,"You're not an operator in that channel"), 1);

    if (_channel->hasUser(new_clientFd))
        return(sendError(client->getClientfd(), "443", new_client->getNickName(), "is already on channel"),1);

    _channel->addToContainer(new_client, _channel->getMembers());

    sendToClient(client, "341 " + client->getNickName() + " " + new_client->getNickName() + " " + channelName);
    std::string announce = ":" + client->getNickName() + "!" + client->getUserName() + "@localhost " + "INVITE " + new_client->getNickName() + " " + channelName;
    sendToClient(new_client, announce);
    return 0;
}

int 	Server::handleKick(Client* client, const std::vector<std::string>& params){//remove from operators also
    for (size_t i = 0; i < params.size(); i++)
        std::cout << params[i] << std::endl;

    if (params.empty() || params.size() != 3)
         return (sendError(client->getClientfd(), "461", "KICK" ,"Not enough parameters"), 1);

    Channel *_channel = findChannel(params[0]);

    if (_channel == NULL)
        return (sendError(client->getClientfd(), "403", params[0], "No such channel"), 1);

    int new_clientFd = getclientfd(params[1]);

    if (new_clientFd == -1)
        return (sendError(client->getClientfd(), "401", params[1],"No such nick"), 1);
    if (!_channel->hasUser(client->getClientfd()))
        return (sendError(client->getClientfd(), "442", params[0],"You're not on that channel"), 1);

    if (!_channel->isOperator(client->getClientfd()))
        return (sendError(client->getClientfd(), "482", params[0] ,"You're not an operator in that channel"), 1);

    Client *new_client = getClient(new_clientFd);

    if (!_channel->hasUser(new_clientFd))
        return(sendError(client->getClientfd(), "441", new_client->getNickName(), "They aren't on that channel"),1);

    std::string comment = (params.size() > 2 && !params[2].empty()) ? params[2] : "You have been kicked from the channel: Grow UP!";
    std::string announce = ":" + client->getNickName() + "!" + client->getUserName() + "@localhost KICK " + params[0] + " " + new_client->getNickName() + " :" + comment;
    
    sendToClient(new_client, announce);
    this->broadcastInChannel(_channel->getMembers(), announce);// need to solve must exclude one (i don't know yet the issue)
    
    _channel->deleteFromContainer(new_client, _channel->getMembers());
    return 0;
}

#include "../includes/Server.hpp"
#include "../includes/Channels.hpp"

static std::vector<std::string> ft_split(std::string str)
{
    size_t start = 0;
    size_t end = 0;
    std::vector<std::string> words;

    while ((end = str.find(',', start)) != std::string::npos) {
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
        std::vector<std::string> canals = ft_split(params[0]);
        for (size_t i = 0; i < canals.size(); i++)
            createChannel(client, canals[i]);
    }
    else
        createChannel(client, params[0]);
    return (0);
}

//int Server::handlePart(Client* client, const std::vector<std::string>& params)
//{
//    if (params.empty())
//        return (this->sendToClient(client, "461 :Not enough parameters"), 1);
//    int chanIndex = this->isChannelExist(params[0]);
//    if (chanIndex == -1)
//        return (this->sendToClient(client, "403 :No such channel"), 1);
//    int userIndex = this->findUser(client->getNickName(), this->chanPool[chanIndex]->getMembers());
//    if (userIndex == -1)
//        return (this->sendToClient(client, "442 :You're not on that channel"), 1);
//    std::string reason = (params.size() > 1) ? params[1] : "See Later Guys";
//    std::string message = ":" + client->getNickName() + "!" + client->getUserName() + "@localhost PART " + params[0] + " :" + reason + "\r\n";
//    this->sendMsgToChannel(client, this->chanPool[chanIndex]->getMembers(), message);
//    this->chanPool[chanIndex]->deleteFromContainer(this->chanPool[chanIndex]->getMembers()[userIndex], this->chanPool[chanIndex]->getMembers());
//    int opIndex = this->findUser(client->getNickName(), this->chanPool[chanIndex]->getOperators());
//    if (this->chanPool[chanIndex]->getOperators().size() == 1 && opIndex != -1)
//    {
//        if (this->chanPool[chanIndex]->getMembers().size() > 1)
//            this->chanPool[chanIndex]->addToContainer(this->chanPool[chanIndex]->getMembers()[0], this->chanPool[chanIndex]->getOperators());
//        else
//        {
//            delete this->chanPool[chanIndex];
//            this->chanPool.erase(this->chanPool.begin() + chanIndex);
//        }
//    }
//    else if (opIndex != -1)
//        this->chanPool[chanIndex]->deleteFromContainer(this->chanPool[chanIndex]->getOperators()[opIndex], this->chanPool[chanIndex]->getOperators());
    
//    return (0);
//}

int		Server::handleMode(Client* client, const std::vector<std::string>& params)
{
    if (params.empty())
        return (this->sendToClient(client, "461 :Not enough parameters"), 1);
    if (numberOfParameterizedArgs(params[1]) > (params.size() - 2))
        return (this->sendToClient(client, "461 :Not enough parameters"), 1);
    int chanIndex = this->isChannelExist(params[0]);
    if (chanIndex == -1)
        return (this->sendToClient(client, "403 :No such channel"), 1);
    if (this->findUser(client->getNickName(), chanPool[chanIndex]->getOperators()) == -1)
        return (this->sendToClient(client, "482 " + params[0] + " :You're not channel operator"), 1);
    // if (params[1].size() > 2)
        // hundleJoinededArgs();
    // hundleShuffledArgs();

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
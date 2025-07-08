#include "../../includes/Server.hpp"

int		Server::handleTopic(Client* client, const std::vector<std::string>& params)
{
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
            time_t timestamp = _channel->getTopicSetAt();
            char buffer[64];
            std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&timestamp));
            sendToClient(client, "332 "+ client->getNickName() + " " + channelName + " :" + _channel->getTopic());
            sendToClient(client, "333 " + channelName + " " + _channel->getTopicSetBy() + " " + buffer);
            return (1);
        }
    }
    std::string topic;
    if (params.size() > 1){
        if (_channel->getRestrictedTopic() && !_channel->isOperator(client->getClientfd()))
            return (sendError(client->getClientfd(), "482", channelName ,"You're not an operator in that channel"), 1);
        topic = params[1];
        _channel->setTopic(topic);
        _channel->setTopicSetBy(client->getNickName());
        _channel->setTopicSetAt(std::time(0));
        _channel->setTopicSetAt(time(0));
    }

    std::string message = ":" + client->getNickName() + "!" + client->getUserName() + "@localhost" + " TOPIC " + channelName + " :" + topic;
    this->broadcastInChannel(_channel->getMembers(), message);
    return 0;
}
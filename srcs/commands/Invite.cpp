#include "../../includes/Server.hpp"

int		Server::handleInvite(Client *client, const std::vector<std::string> &params)
{
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

    new_client->createInvite(channelName, true);
    sendToClient(client, "341 " + client->getNickName() + " " + new_client->getNickName() + " " + channelName);
    std::string announce = ":" + client->getNickName() + "!" + client->getUserName() + "@localhost " + "INVITE " + new_client->getNickName() + " " + channelName;
    // broadcastInChannel(_channel->getMembers(), announce, *client);
    sendToClient(new_client, announce);
    return 0;
}
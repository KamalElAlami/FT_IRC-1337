#include "../../includes/Server.hpp"

int 	Server::handleKick(Client* client, const std::vector<std::string>& params)
{
    for (size_t i = 0; i < params.size(); i++)
        std::cout << "params[" << i << "] = " << params[i] << std::endl;
    if (params.empty() || params.size() < 2)
         return (sendError(client->getClientfd(), "461", "KICK" ,"Not enough parameters"), 1);

    Channel *_channel = findChannel(params[0]);

    if (_channel == NULL)
        return (sendError(client->getClientfd(), "403", params[0], "No such channel"), 1);
    
    if (!_channel->isOperator(client->getClientfd()))
        return (sendError(client->getClientfd(), "482", params[0] ,"You're not an operator in that channel"), 1);
        
    if (!_channel->hasUser(client->getClientfd()))
        return (sendError(client->getClientfd(), "442", params[0],"You're not on that channel"), 1);
    
    std::string targetName;
    std::stringstream ss(params[1]);
    while(std::getline(ss, targetName, ',')) {
        // if (targetName.empty())
        //     return (sendError(client->getClientfd(), "461", "KICK", "Not enough parameters"), 1);
        int new_clientFd = getclientfd(targetName);
            
        if (new_clientFd == -1){
            sendError(client->getClientfd(), "401", targetName,"No such nick");
            continue;
        }
            
            
        Client *new_client = getClient(new_clientFd);
            
        if (!_channel->hasUser(new_clientFd)){
            sendError(client->getClientfd(), "441", new_client->getNickName(), "They aren't on that channel");
            continue;
        }
            
        std::string comment = (params.size() > 2 && !params[2].empty()) ? params[2] : "You have been kicked from the channel: Grow UP!";
        std::string announce = ":" + client->getNickName() + "!" + client->getUserName() + "@localhost KICK " + params[0] + " " + new_client->getNickName() + " :" + comment;
            
        sendToClient(new_client, announce);
        this->broadcastInChannel(_channel->getMembers(), announce);
            
        if (this->findUser(new_client->getNickName(), _channel->getOperators()) != -1)
        _channel->deleteFromContainer(new_client, _channel->getOperators());
        _channel->deleteFromContainer(new_client, _channel->getMembers());
    }

    return 0;
}

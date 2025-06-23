#include "../../includes/Server.hpp"

void Server::displayModes(Client* client, Channel* channel)
{
    std::string modes = "+";
    std::string Params = "";
    
    if (channel->getInviteOnly()) modes += "i";
    if (channel->getRestrictedTopic()) modes += "t";
    if (channel->getMemberLimit() != -1)
    {
        modes += "l";
        std::ostringstream limit;
        limit << channel->getMemberLimit();
        Params += " " + limit.str();
    }
    if (channel->getEnabledPass())
    {
        modes += "k";
        Params += " " + channel->getPassword();
    }
    sendToClient(client, "324 " + client->getNickName() + " " + channel->getName() + " " + modes + Params);
    
    // RPL_CREATIONTIME (329) - next khasni n implement creation time to send 329 rpl

}

int		Server::handleMode(Client* client, const std::vector<std::string>& params)
{
    // (void)client;
    for(size_t i = 0; i < params.size(); i++){
        std::cout << "params de " << i << "is :" << params[i] << std::endl;
    }
    if (params.size() < 1)
        return (sendError(client->getClientfd(), "461", "MODE" ,"Not enough parameters"), 1);
    if (params[0][0] != '#')
        return (1);
    std::string channelName = params[0];
    Channel *_channel = findChannel(channelName);
        
    if (!_channel)
        return (sendError(client->getClientfd(), "403", channelName, "No such channel"), 1);
        
    if (params.size() == 1)
        return (displayModes(client, _channel), 1);

    if (!_channel->isOperator(client->getClientfd()))
        return (sendError(client->getClientfd(), "482", channelName ,"You're not an operator in that channel"), 1);
    
    std::string modestring = params[1];
    bool mode = true;
    size_t argindex = 2;
    
    for (size_t i = 0; i < modestring.size(); i++){
        if (modestring[i] == 'i')
            _channel->setInviteOnly(mode);
        else if (modestring[i] == 't')
            _channel->setRestrictedTopic(mode);
        else if (modestring[i] == 'k')
            handelkeymode(client, params, argindex, mode, *_channel);
        else if (modestring[i] == 'l')
            setUserLimit(client, params, argindex, mode, *_channel);
        else if (modestring[i] == 'o')
            handelChannelOperator(client, params, argindex, mode, *_channel);
        else if (modestring[i] == '-')
            mode = false;
        else if (modestring[i] == '+')
            mode = true;
    }
    std::cout << mode << std::endl;
    
    return (0);
}

void Server::handelChannelOperator(Client *client, const std::vector<std::string>&params, size_t &argindex, bool mode, Channel &_channel){
    if (params.size() <= argindex){
        sendError(client->getClientfd(), "461", "MODE +o", "Not enough parameters");
        return;
    }
    std::string targetName = params[argindex];
    int target_fd = getclientfd(targetName);

    if (target_fd == -1){
        sendError(client->getClientfd(), "401", params[1],"No such nick");
        argindex++;
        return;
    }
    Client *target = getClient(target_fd);
    if (!_channel.hasUser(target_fd)){
        sendError(client->getClientfd(), "441", target->getNickName(), "They aren't on that channel");
        argindex++;
        return;
    }
    if (mode){
        if (_channel.isOperator(target_fd)){
            sendError(client->getClientfd(), "482", _channel.getName() ,"is already an operator");
            argindex++;
            return;
        }
        _channel.addToContainer(target,_channel.getOperators());
    }
    else{
        if (!_channel.isOperator(target_fd)){
            sendError(client->getClientfd(), "482", _channel.getName() ,"is not an operator");
            argindex++;
            return;
        }
        _channel.deleteFromContainer(target,_channel.getOperators());
    }
    argindex++;
}

bool isValidNumber(std::string n){
    for (size_t i = 0; i < n.size(); i++){
        if (!isdigit(n[i]))
            return false;
    }
    return true;
}

void Server::setUserLimit(Client *client, const std::vector<std::string>&params, size_t &argindex, bool mode, Channel &_channel){
    if (mode){
        if (params.size() <= argindex){
            sendError(client->getClientfd(), "461", "MODE +l", "Not enough parameters");
            return;
        }
        std::string limit = params[argindex];
        if (!isValidNumber(limit)){
            sendError(client->getClientfd(), "461", "MODE +l", "argument error");
            argindex++;
            return;
        }
        _channel.setMemberLimit(std::atoi(limit.c_str()));
        argindex++;
    }
    else{
        _channel.setMemberLimit(-1);
    }
    std::cout << "user limits: " << _channel.getMemberLimit() << std::endl;
}

void Server::handelkeymode(Client *client, const std::vector<std::string>&params, size_t &argindex, bool mode, Channel &_channel){
    if (mode){
        if (params.size() <= argindex){
            sendError(client->getClientfd(), "461", "MODE +k", "Not enough parameters");
            return;
        }
        std::string key = params[argindex];
        _channel.setPassword(key);
        _channel.setEnabledPass(true);
        argindex++;
    }
    else{
        if (params.size() > argindex)
            argindex ++;
        _channel.setPassword("");
        _channel.setEnabledPass(false);
    }
    std::cout << "channel key: " << _channel.getPassword() << std::endl;

}
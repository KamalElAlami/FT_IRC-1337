#include "../../includes/Server.hpp"

void Server::displayModes(Client* client, Channel* channel)
{
    std::string modes = "+";
    std::string Params = "";
    std::ostringstream creationTime;
    creationTime << channel->getCreationTime();
    
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
    sendToClient(client, "329 " + client->getNickName() + " " + channel->getName() + " " + creationTime.str());

}

int		Server::handleMode(Client* client, const std::vector<std::string>& params)
{
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
    bool status = false;
    size_t argindex = 2;
    std::string modechar;
    
    for (size_t i = 0; i < modestring.size(); i++){
        if (modestring[i] == 'i'){
            _channel->setInviteOnly(mode);
            status = true;
        }
        else if (modestring[i] == 't'){
            _channel->setRestrictedTopic(mode);
            status = true;
        }
        else if (modestring[i] == 'k')
            handelkeymode(client, params, argindex, mode, *_channel, status);
        else if (modestring[i] == 'l')
            setUserLimit(client, params, argindex, mode, *_channel, status);
        else if (modestring[i] == 'o')
            handelChannelOperator(client, params, argindex, mode, *_channel, status);
        else if (modestring[i] == '-')
            mode = false;
        else if (modestring[i] == '+')
            mode = true;
        else{
            modechar += modestring[i];
            std::cout << "the mode is " << modechar << std::endl;
            sendError(client->getClientfd(), "472",client->getNickName() + " " + modechar, ":is unknown mode char to me");
            modechar.erase();
        }
        if(status){
            std::string announce = ":" + client->getNickName() + "!" + "localhost MODE "
             + channelName + " " + (mode ? "+" : "-") + modestring[i];
            if(modestring[i] == 'o' || (mode && modestring[i] == 'l') || (mode && modestring[i] == 'k'))
                announce += " " + params[argindex - 1];
            std::cout << announce << std::endl;
            broadcastInChannel(_channel->getMembers(), announce);
            status = false;
        }
    }

    return (0);
}

void Server::handelChannelOperator(Client *client, const std::vector<std::string>&params, size_t &argindex, bool mode, Channel &_channel, bool &status){
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
        if (target_fd == client->getClientfd()){
            sendError(target_fd, "482", _channel.getName(), "You can't remove your own operator status");
            argindex++;
            return;
        }
        _channel.deleteFromContainer(target,_channel.getOperators());
    }
    argindex++;
    status = true;
}

bool isValidNumber(std::string n){
    for (size_t i = 0; i < n.size(); i++){
        if (!isdigit(n[i]))
            return false;
    }
    return true;
}

void Server::setUserLimit(Client *client, const std::vector<std::string>&params, size_t &argindex, bool mode, Channel &_channel, bool &status){
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
    status = true;
}

void Server::handelkeymode(Client *client, const std::vector<std::string>&params, size_t &argindex, bool mode, Channel &_channel, bool &status){
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
    status = true;
    std::cout << "channel key: " << _channel.getPassword() << std::endl;
}
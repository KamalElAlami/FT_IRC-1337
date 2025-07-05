#include "../../includes/Server.hpp"

int Server::handelQuit(Client* client, const std::vector<std::string>& params){
    std::vector<Channel *>::iterator it;
    for (it = this->chanPool.begin(); it != this->chanPool.end();){
        (*it)->deleteFromContainer(client, (*it)->getMembers());
        (*it)->deleteFromContainer(client, (*it)->getOperators());
        std::string message = ":" + client->getNickName() + "!" + client->getUserName() + "@localhost PART " + (*it)->getName() + " :" + params[0] + "\r\n";
        broadcastInChannel((*it)->getMembers(), message);
        if ((*it)->getOperators().empty()){
            if ((*it)->getMembers().size() >= 1){
                (*it)->addToContainer((*it)->getMembers()[0], (*it)->getOperators());
                std::string announce = ":" + client->getNickName() + "!" + "localhost MODE "
                + (*it)->getName() + " +o " + (*it)->getMembers()[0]->getNickName();
             broadcastInChannel((*it)->getMembers(), announce);
             it++;
            }
            else{
                delete(*it);
                it = this->chanPool.erase(it);
                continue;
            }
        }
        else
            it++;
    }
    return 0;
}
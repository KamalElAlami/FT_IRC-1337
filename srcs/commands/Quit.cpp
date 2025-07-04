#include "../../includes/Server.hpp"

int Server::handelQuit(Client* client, const std::vector<std::string>& params){
    (void)params;
    std::vector<Channel *>::iterator it;
    for (it = this->chanPool.begin(); it != this->chanPool.end(); it++){
        (*it)->deleteFromContainer(client, (*it)->getMembers());
        (*it)->deleteFromContainer(client, (*it)->getOperators());
        if ((*it)->getOperators().empty()){
            if ((*it)->getMembers().size() > 1)
                (*it)->addToContainer((*it)->getMembers()[0], (*it)->getOperators());
            else{
                delete (*it);
                this->chanPool.erase(it);
            }
        }
    }
    return 0;
}
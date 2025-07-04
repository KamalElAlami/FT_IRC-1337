#include "../../includes/Server.hpp"

int Server::handelQuit(Client* client, const std::vector<std::string>& params){
    (void)params;
    std::vector<Channel *>::iterator it;
    for (it = this->chanPool.begin(); it != this->chanPool.end(); it++){
        (*it)->deleteFromContainer(client, (*it)->getMembers());
        (*it)->deleteFromContainer(client, (*it)->getOperators());
    }
    return 0;
}
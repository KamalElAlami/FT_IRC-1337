#include "../../includes/Server.hpp"

int Server::handleQuit(Client* client, const std::vector<std::string>& params)
{
    std::string announce;
    std::string reason;
    
    if (!params.size())
        reason = "Client exited";
    else
        reason = params[0];
    announce = ":" + client->getNickName() + "!" + client->getUserName() + "@" + client->getHostName() + " QUIT :" + reason;
    // std::cout << announce << std::endl;
    for (size_t i = 0; i < chanPool.size(); i++)
    {
        int userIndex = findUser(client->getNickName(), chanPool[i]->getMembers());
        if (userIndex != -1)
        {
            broadcastInChannel(this->chanPool[i]->getMembers(), announce);
            this->chanPool[i]->deleteFromContainer(this->chanPool[i]->getMembers()[userIndex], this->chanPool[i]->getMembers());
            int opIndex = this->findUser(client->getNickName(), this->chanPool[i]->getOperators());
            if (this->chanPool[i]->getOperators().size() == 1 && opIndex != -1)
            {
                if (this->chanPool[i]->getMembers().size() >= 1)
                {
                    this->chanPool[i]->addToContainer(this->chanPool[i]->getMembers()[0], this->chanPool[i]->getOperators());
                    std::string announce = ":" + client->getNickName() + "!" + "localhost MODE "
                    + this->chanPool[i]->getName() + " +o " + this->chanPool[i]->getMembers()[0]->getNickName();
                    broadcastInChannel(this->chanPool[i]->getMembers(), announce);
                }
                else
                {
                    delete this->chanPool[i];
                    this->chanPool.erase(this->chanPool.begin() + i);
                }
            }
            else if (opIndex != -1)
                this->chanPool[i]->deleteFromContainer(this->chanPool[i]->getOperators()[opIndex], this->chanPool[i]->getOperators());
        }
    }
    // i have to check this part with oussama
    int clientIdx = findUser(client->getNickName(), this->clients);
    if (clientIdx != -1)
    {
        close(client->getClientfd());
        delete clients[clientIdx];
        clients.erase(clients.begin() + clientIdx);
    }
    return (0);
}
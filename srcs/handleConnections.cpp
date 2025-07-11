#include "../includes/Server.hpp"

void Server::handleNewConnection()
{
	Client _client;
	struct pollfd client_pollfd;
	struct sockaddr_in addr = {};
	socklen_t addr_len = sizeof(addr);

	_client.setClientfd(accept(this->SerSockFd, (struct sockaddr *)&addr, &addr_len));
	if (_client.getClientfd() == -1) {
		std::cerr << "Worning: Accept: Failed to accept new connection using accept()" << std::endl;
		return ;
	}

	_client.setAddress(inet_ntoa(addr.sin_addr));
	Client *tmp = new Client(_client);
	this->clients.push_back(tmp);
	client_pollfd.fd = _client.getClientfd();
	client_pollfd.events = POLLIN;
	client_pollfd.revents = 0;
	this->polling.push_back(client_pollfd);
}


Client * Server::getClient(int clientfd)
{
	for (size_t i = 0; i < this->clients.size(); i++)
	{
		if (this->clients[i]->getClientfd() == clientfd)
			return (this->clients[i]);
	}
	return (NULL);
}

std::string Server::EraseLine(std::string & src, int pos)
{
	std::string res = "";

	if (src.find("\r\n") != std::string::npos)
	{
		pos--;
		res = src.substr(0, pos);
		src.erase(0, pos + 2);
	}
	else if (src.find("\n") != std::string::npos)
	{
		res = src.substr(0, pos);
		src.erase(0, pos + 1);
	}
	return res;
}

void Server::handleClientMessage(int clientFd)
{
	std::string	line;
	std::string	data;
	size_t		position;
	Client		*client;

	client = getClient(clientFd);
	if (!client || client->getRemoveClient() == true)
	{
		if(client)
			this->handleClientDisconnect(clientFd);
		return ;
	}
	client->CustomBuffer();
	if (client->getRemoveClient() == true)
	{
		this->handleClientDisconnect(clientFd);
		return ;
	}
	data = client->getBuffer();
	while ((position = data.find("\n")) != std::string::npos && client)
	{
		line = EraseLine(data, position);
		client->setBuffer(data);
		if (!line.empty())
			this->ParseCommand(client, line);
	}
}

void	Server::removeClient(int clientFd)
{
	for (size_t i = 0; i < this->clients.size(); i++)
	{
		if (this->clients[i]->getClientfd() == clientFd)
		{
			close (clientFd);
			delete this->clients[i];
			this->clients.erase(this->clients.begin() + i);
			break;
		}
	}
	for (size_t i = 0; i < this->polling.size(); i++)
	{
		if (this->polling[i].fd == clientFd)
		{
			this->polling.erase(this->polling.begin() + i);
			break;
		}
	}
}

void Server::kickClient(int fd)
{
	Client *client = getClient(fd);
	std::vector<Channel *>::iterator it;
    for (it = this->chanPool.begin(); it != this->chanPool.end();){
        (*it)->deleteFromContainer(client, (*it)->getMembers());
        (*it)->deleteFromContainer(client, (*it)->getOperators());
		std::string announce = ":" + client->getNickName() + "!" + client->getUserName() + "@" + client->getHostName() + " QUIT :leaving!";
		broadcastInChannel((*it)->getMembers(), announce);
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
}

void Server::handleClientDisconnect(int clientFd)
{
	kickClient(clientFd);
	this->removeClient(clientFd);
	std::cout << "Client " << clientFd << " disconnected" << std::endl;
}
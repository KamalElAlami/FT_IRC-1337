#include "../includes/Server.hpp"

void Server::handleNewConnection()
{
	Client _client;
	struct pollfd client_pollfd;
	struct sockaddr_in addr = {};
	socklen_t addr_len = sizeof(addr);

	_client.Clientfd = accept(this->SerSockFd, (struct sockaddr *)&addr, &addr_len);
	if (_client.Clientfd == -1)
		throw std::runtime_error( "Error: Failed to accept new connection using accept()");

	_client.address = inet_ntoa(addr.sin_addr);
	Client *tmp = new Client(_client);
	this->clients.push_back(tmp);
	client_pollfd.fd = _client.Clientfd;
	client_pollfd.events = POLLIN;
	client_pollfd.revents = 0;
	this->polling.push_back(client_pollfd);
}

void Server::handleClientMessage(int clientFd)
{
	int				BytesRead;
	char			buffer[1024];
	size_t			pos = 0;
	Client			*client;
	std::string		line;

	for (size_t i = 0; i < this->clients.size(); i++)
	{
		if (this->clients[i]->Clientfd == clientFd)
		{
			client = this->clients[i];
			break;
		}
	}

	memset(buffer, 0, 1024);
	BytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, MSG_DONTWAIT);
	std::cout << "Buffer : " << buffer << std::endl;
	if (BytesRead == 0)
		this->handleClientDisconnect(clientFd);
	else if (BytesRead < 0)
		throw std::runtime_error( "Error: Failed to receive data from socket using recv()");
	
	buffer[BytesRead] = '\0';
	std::string		message(buffer);
	while ((pos = message.find("\r\n")) != std::string::npos)
	{
		line = message.substr(0, pos);
		message.erase(0, pos + 2);
		if (!line.empty())
			this->ParseCommand(client, line);
	}
}

void	Server::removeClient(int clientFd)
{
	for (size_t i = 0; i < this->clients.size(); i++)
	{
		if (this->clients[i]->Clientfd == clientFd)
		{
			delete this->clients[i];
			this->clients[i] = NULL;
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
	close (clientFd);
}

void Server::handleClientDisconnect(int clientFd)
{
	this->removeClient(clientFd);
	std::cout << "Client " << clientFd << " disconnected" << std::endl;
}
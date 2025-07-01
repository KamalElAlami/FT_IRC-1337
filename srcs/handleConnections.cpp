#include "../includes/Server.hpp"

void Server::handleNewConnection()
{
	Client _client;
	struct pollfd client_pollfd;
	struct sockaddr_in addr = {};
	socklen_t addr_len = sizeof(addr);

	_client.setClientfd(accept(this->SerSockFd, (struct sockaddr *)&addr, &addr_len));
	if (_client.getClientfd() == -1)
		throw std::runtime_error( "Error: Failed to accept new connection using accept()");

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








//void Server::handleClientMessage(int clientFd)
//{
//	int				BytesRead;
//	char			buffer[1024];
//	size_t			pos = 0;
//	Client			*client;
//	std::string		line;

//	for (size_t i = 0; i < this->clients.size(); i++)
//	{
//		if (this->clients[i]->getClientfd() == clientFd)
//		{
//			client = this->clients[i];
//			break;
//		}
//	}

//	memset(buffer, 0, 1024);
//	BytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, MSG_DONTWAIT);
//	//std::cout << "Buffer : " << buffer << std::endl;
//	if (BytesRead == 0)
//		this->handleClientDisconnect(clientFd);
//	else if (BytesRead < 0)
//		throw std::runtime_error( "Error: Failed to receive data from socket using recv()");

//	buffer[BytesRead] = '\0';
//	std::string		message(buffer);
//	//std::cout << "Before: Message : " << message << ", line : " << line << std::endl;
//	while ((pos = message.find("\r\n")) != std::string::npos && client)
//	{
//		line = message.substr(0, pos);
//		message.erase(0, pos + 2);
//		if (!line.empty())
//			this->ParseCommand(client, line);
//	}
//}

void	Server::removeClient(int clientFd)
{
	for (size_t i = 0; i < this->clients.size(); i++)
	{
		if (this->clients[i]->getClientfd() == clientFd)
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
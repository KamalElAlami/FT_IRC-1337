#include "../includes/Server.hpp"

/*---------------------- Canonical orthodox form ----------------------*/
Server::Server(int _fd, int _Port) : SerSockFd(_fd), Port(_Port) {}
Server::~Server() {}
Server::Server(Server const & src)
{
	*this = src;
}
Server & Server::operator=(Server const & src)
{
	this->SerSockFd = src.SerSockFd;
	this->Port = src.Port;
	return *this;
}

/*---------------------- Seter's and geter's ----------------------*/
void	Server::set_SerSockFd(int fd) {
	this->SerSockFd = fd;
}
void	Server::set_Port(int _Port) {
	this->Port = _Port;
}
void	Server::set_Password(std::string _Pass) {
	this->Password = _Pass;
}
int Server::get_SerSockFd() const {
	return (this->SerSockFd);
}
int	Server::get_Port() const {
	return (this->Port);
}


/*---------------------- Server method's ----------------------*/

void Server::Start_Server()
{
	try {
		this->Build_Server();
		while (true)
		{
			if (poll(this->polling.data(), this->polling.size(), -1) == -1)
			throw std::runtime_error( "Error: Failed to monitor file descriptors using poll()");
			
			for (size_t i = 0; i < this->polling.size(); i++)
			{
				if (this->polling[i].revents & POLLIN)
				{
					if(this->polling[i].fd == this->SerSockFd)
						this->handleNewConnection();
					else
					{
						this->handleClientMessage(this->polling[i].fd);
						// handlClientMessage
					}
				}
			}
		}
	}
	catch(const std::exception& e) {
		// dont forget to close all FDs
		std::cerr << e.what() << std::endl;
		exit (1);
	}
}

void Server::Build_Server()
{
	struct sockaddr_in addr;
	struct pollfd poller;

	this->SerSockFd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0); // check if multy flaging is forb
	if (this->SerSockFd == -1)
		throw std::runtime_error( "Error: Failed to create socket using socket()");

	addr.sin_family = AF_INET;
	addr.sin_port = htons(this->Port);
	addr.sin_addr.s_addr = INADDR_ANY;

	poller.fd = this->SerSockFd;
	poller.events = POLLIN;
	this->polling.push_back(poller);

	int opt = 1;
	if (setsockopt(this->SerSockFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		throw std::runtime_error( "Error: Failed to set socket options using setsockopt()");

	if (bind(this->SerSockFd, (sockaddr *)&addr, sizeof(addr)))
		throw std::runtime_error( "Error: Failed to bind socket to address using bind()");

	if (listen(this->SerSockFd, SOMAXCONN))
		throw std::runtime_error( "Error: Failed to set socket to listening state using listen()");

	std::cout << "Server Start on Port < " << this->Port 
	<< " >, Password < " << this->Password << " >" << std::endl;
}

void Server::handleNewConnection()
{
	Client _client;
	struct pollfd client_pollfd;
	struct sockaddr_in addr = {};
	socklen_t addr_len = sizeof(addr);

	_client.Clientfd = accept(this->SerSockFd, (struct sockaddr *)&addr, &addr_len);
	if (_client.Clientfd == -1)
	{
		throw std::runtime_error( "Error: Failed to accept new connection using accept()");
	}
	
		_client.address = inet_ntoa(addr.sin_addr);
	Client *tmp = new Client(_client); // We should should delet it
	this->clients.push_back(tmp);
	client_pollfd.fd = _client.Clientfd;
	client_pollfd.events = POLLIN;
	client_pollfd.revents = 0;
	this->polling.push_back(client_pollfd);
	std::cout << "New connection from Client < " << _client.Clientfd
	<< " >, Address < "<< _client.address << " >" << std::endl;
}

void Server::handleClientMessage(int clientFd)
{
	int			BytesRead;
	size_t		pos = 0;
	char		buffer[1024];

	memset(buffer, 0, 1024);
	BytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, MSG_DONTWAIT);
	// std::cout << "Buffer : " << buffer << std::endl;
	if (BytesRead == 0)
		this->handleClientDisconnect(clientFd);
	else if (BytesRead < 0)
		throw std::runtime_error( "Error: Failed to receive data from socket using recv()");
	
	buffer[BytesRead] = '\0';
	std::string		message(buffer);
	std::string		line;
	while ((pos = message.find("\r\n")) != std::string::npos)
	{
		line = message.substr(0, pos);
		message.erase(0, pos + 2);
		if (!line.empty())
		{
			std::cout << "Received from client " << clientFd << ": " << line << std::endl;
			// add her commend parsing;
		}
	}
	
}

void Server::handleClientDisconnect(int clientFd)
{
	for (size_t i = 0; i < this->clients.size(); i++)
	{
		if (this->clients[i]->Clientfd == clientFd)
		{
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

	close (clientFd);

	std::cout << "Client " << clientFd << " disconnected" << std::endl; // u can add this to client destructor
}


std::vector<std::string> Server::splitBySpaces(const std::string& middle)
{
	std::vector<std::string>	params;
	size_t						start = 0, end;

	while ((end = middle.find(' ', start)) != std::string::npos)
	{
		if (end > start)
			params.push_back(middle.substr(start, end - start));
		start = end + 1;
	}
	if (start < middle.length())
		params.push_back(middle.substr(start));

    return (params);
}



void	Server::ParseCommand(int clientFd, std::string const & line)
{
	size_t						CmdPos = 0, ParamStart = 0;
	size_t						start = 0, end = 0;
	std::string					Command;
	std::string					middle;
	std::vector <std::string>	params;

	CmdPos = line.find(' ');
	if (CmdPos == std::string::npos)
		Command = line;
	else
		Command = line.substr(0, CmdPos);
	for (size_t i = 0; i < Command.length(); i++)
		Command[i] = toupper(Command[i]);

	if (CmdPos != std::string::npos)
	{
		ParamStart = CmdPos + 1;
		size_t triStart = line.find(" :", ParamStart);
		if (triStart != std::string::npos)
		{
			middle = line.substr(ParamStart, triStart - ParamStart);
			params = this->splitBySpaces(middle);
			params.push_back(line.substr(triStart + 2));
		}
		else
		{
			middle = line.substr(ParamStart);
			params = this->splitBySpaces(middle);
		}
	}
}

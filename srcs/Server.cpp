#include "../includes/Server.hpp"
#include <fcntl.h> 

Server::Server(int _fd, int _Port) : SerSockFd(_fd), Port(_Port) {
}
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
						this->handleClientMessage(this->polling[i].fd);
				}
			}
		}
	}
	catch(const std::exception& e)
	{
		this->ClearAll();
		std::cerr << e.what() << std::endl;
		exit (1);
	}
}

void Server::Build_Server()
{
	struct sockaddr_in addr;
	struct pollfd poller;

	this->SerSockFd = socket(AF_INET, SOCK_STREAM, 0);
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
		throw std::runtime_error( "Error: Failed to set socket options using setsockopt()");\
	
	if (fcntl(this->SerSockFd, F_SETFL, O_NONBLOCK) == -1)
		throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));

	if (bind(this->SerSockFd, (sockaddr *)&addr, sizeof(addr)))
		throw std::runtime_error( "Error: Failed to bind socket to address using bind()");

	if (listen(this->SerSockFd, SOMAXCONN))
		throw std::runtime_error( "Error: Failed to set socket to listening state using listen()");

	std::cout << "IRC Server Start on Port < " << this->Port 
	<< " >, Password < " << this->Password << " >" << std::endl;
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

void Server::ClearAll()
{
	for (int i = this->clients.size() - 1; i > -1; i--)
	{
		delete this->clients[i];
		this->clients[i] = NULL;
		this->clients.erase(this->clients.begin() + i);
	}

	for (size_t i = 0; i < this->polling.size(); i++)
		close(this->polling[i].fd);
}



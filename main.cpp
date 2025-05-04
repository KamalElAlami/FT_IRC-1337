#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Server.hpp"
#include <sys/types.h>
// #include "Client.hpp"

void Server::craeteSocket()
{
	int opt = 1;
	struct sockaddr_in addr = {};
	struct pollfd file_descriptor;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(this->port);
	addr.sin_addr.s_addr = INADDR_ANY;

	this->SocketFd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (this->SocketFd == -1)
		throw std::runtime_error("Error creating socket\n");

	file_descriptor.fd = this->SocketFd;
	file_descriptor.events = POLLIN;
	this->polling_files.push_back(file_descriptor);

	if(setsockopt(this->SocketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		throw std::runtime_error("Error set Socket option: setsockopt()\n");

	if(bind(this->SocketFd, (sockaddr *)&addr, sizeof(addr)))
		throw std::runtime_error("Error set Socket option: setsockopt()\n");

	if (listen(this->SocketFd, SOMAXCONN))
		throw std::runtime_error("Error in witing new connections: listen()\n");
	// add her that server has been connected!
	std::cout << "The Server has been Connected!"<< std::endl;

	for (;;) // this a true loop (Infinite loop)
	{
		int ready = poll(this->polling_files.data(), this->polling_files.size(), -1);
		if (ready == -1)
		{
			perror("poll");
			exit(EXIT_FAILURE);
		}
		for(size_t i = 0; i < this->polling_files.size(); i++)
		{
			if (this->polling_files[i].revents & POLLIN && this->polling_files[i].fd == this->SocketFd)
			{
				//add her fanction that handel new connection
				Client cln;
				struct pollfd client_pollfd;

				struct sockaddr_in addr2 = {};
				socklen_t addr2_len = sizeof(addr2);
				cln.ClientFd = accept(this->SocketFd, (sockaddr *)&addr2, &addr2_len);
				if (cln.ClientFd == -1)
				{
					perror("accept");
					exit(1);
				}
				cln.addrs = inet_ntoa(addr2.sin_addr);
				this->clients.push_back(cln);
				client_pollfd.fd = cln.ClientFd;
				client_pollfd.events = POLLIN;
				this->polling_files.push_back(client_pollfd);
				std::cout << "client seccesfuly added to the list!"<< std::endl;
			}
			else if (this->polling_files[i].revents & POLLIN && this->polling_files[i].fd != this->SocketFd)
			{
				// add her fanction thst handel commenecation between server and client
			}
		}

	}	
	
}

int main(int ac, char **av)
{   
	if (ac != 3)
		return (std::cerr << "Usage: " << "<port>" << " <pass>" << std::endl, 1);

	try
	{
		Server svr;
		svr.port = std::stoi(av[1]);
		if (svr.port < 1024 || svr.port > 65535) // replace this condition with port and pass parsing fanction
			throw std::invalid_argument("Port must be between 1024 and 65535\n");
		svr.craeteSocket();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return (0);
}
#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <string>
#include <poll.h>
#include <sys/socket.h>
// #include <netinet/in.h>
#include "Client.hpp"

class Server
{
	private:
	public:
		int port;
		int SocketFd;
		std::string Password;
		std::vector <Client> clients;
		std::vector < struct pollfd> polling_files;
		/*----------------------------------------------------------------------*/
		void craeteSocket();
		Server(){};
		~Server(){};
};


#endif
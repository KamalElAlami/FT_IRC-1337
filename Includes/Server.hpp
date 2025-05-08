#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Client.hpp"
#include <iostream>
#include <cstdlib>
#include <string>
#include <poll.h>
#include <vector>


class Server
{
	private:
		int							SerSockFd;
		int							Port;
		std::string					Password;
		std::vector <Client*>		clients;
		std::vector <struct pollfd>	polling;

	public:
		/*---- Canonical orthodox form ----*/
		Server(int _fd, int _Port);
		~Server();
		Server(Server const & src);
		Server & operator=(Server const & src);

		/*---- Seter's and geter's ----*/
		void	set_SerSockFd(int fd);
		void	set_Port(int _Port);
		int		get_SerSockFd() const;
		int		get_Port() const;
		void	set_Password(std::string _Pass);

		/*---- Server method's ----*/
		void	Start_Server();
		void	Build_Server();
		void	handleNewConnection();
		void	handleClientMessage(int clientFd);
		void	handleClientDisconnect(int clientFd);
};

#endif

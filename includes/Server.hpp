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
#include <cerrno>
#include <cstring>
#include <signal.h>
#include <unistd.h>
#include "./Channels.hpp"


class Server
{
	private:
		int							SerSockFd;
		int							Port;
		static bool						signals;
		std::string						Password;
		std::vector <Client*>			clients;
		std::vector <struct pollfd>		polling;
		std::vector <Channel*>			chanPool;

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
		void	ParseCommand(Client* client, std::string const & line);
		int		handlePass(Client* client, const std::vector<std::string>& params);
		int		handleNick(Client* client, const std::vector<std::string>& params);
		int		handleUser(Client* client, const std::vector<std::string>& params);
		int		handlePingPong(Client* client, const std::vector<std::string>& params);
		int		handlePrivMsg(Client* client, const std::vector<std::string>& params);
		// int		handleTopic(Client* client, const std::vector<std::string>& params);
		int		handleJoin(Client* client, const std::vector<std::string>& params);
		void	removeClient(int ClientFd);
		void	sendToClient(Client* client, const std::string& message);
		void	checkRegistration(Client* client);
		void	ClearAll();
		int		handleCap(Client* client, const std::vector<std::string>& params);
		/*---- Utiles method's ----*/
		std::vector<std::string> splitBySpaces(const std::string& middle);
		bool isChannelExist(std::string chanName);
		void broadcastInChannel(std::vector <Client *> members, std::string message);
		int findChannel(std::string name);
		int findUser(std::string name);

};

#endif

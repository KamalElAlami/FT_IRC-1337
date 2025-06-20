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
#include "Channels.hpp"
#include "chat.hpp"
#include <fstream>


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
		int		handleCap(Client* client, const std::vector<std::string>& params);
		int		handlePingPong(Client* client, const std::vector<std::string>& params);
		int		handlePrivMsg(Client* client, const std::vector<std::string>& params);
		// int		handleTopic(Client* client, const std::vector<std::string>& params);
		int		handlePart(Client* client, const std::vector<std::string>& params);
		int		handleJoin(Client* client, const std::vector<std::string>& params);
		int		handleMode(Client* client, const std::vector<std::string>& params);
		int		handleSbiksla(Client* client, const std::vector<std::string>& params);
		void	createChannel(Client* client, std::string channelName);
		void	removeClient(int ClientFd);
		void	sendToClient(Client* client, const std::string& message);
		void	checkRegistration(Client* client);
		Client*    createBot(void);
		void	ClearAll();
		/*---- Utiles method's ----*/
		std::vector<std::string> splitBySpaces(const std::string& middle);
		int isChannelExist(std::string chanName);
		void broadcastInChannel(std::vector <Client *> members, std::string message);
		void sendMsgToChannel(Client* client, std::vector <Client *> members, std::string message);
		int findUser(std::string name, std::vector <Client*> clients);

};

size_t numberOfParameterizedArgs(std::string arg);

#endif

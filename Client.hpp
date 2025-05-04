#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <vector>
#include <netinet/in.h>

class Client
{
	private:
	public:
	int ClientFd;
	std::string addrs;
	std::string nick;
	std::string user;
	std::string pass;
	bool authenticated;
	Client(){
		addrs = "";
		nick = "";
		pass = "";
		user = "";
		authenticated = false;
	};
		~Client(){};
};


#endif
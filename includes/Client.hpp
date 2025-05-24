#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client
{
	private:
	public:
		int			Clientfd;
		bool		registered;
		std::string	address;
		std::string	nickName;
		std::string	userName;
		std::string	password;
		std::string	realName;
		std::string	hostName;
		bool capNegotiation;    // Track if CAP negotiation is in progress
		bool capEnded;          // Track if client sent CAP END
		Client();
		Client(Client const & src)
		{
			*this = src;
		};
		std::string getNick() const
		{
			return this->nickName;
		};
		~Client(){};
};



#endif

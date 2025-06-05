#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <sys/socket.h>
#include <iostream>

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
		int getClientFd();
		std::string getPrefix()const;


		void sendMessage(const std::string& message) {
        std::string formatted_message = ":" + nickName + " " + message + "\r\n";
        ssize_t bytes_sent = send(Clientfd, formatted_message.c_str(), formatted_message.length(), 0);
        if (bytes_sent == -1) {
            std::cout << "Error sending message to client " << nickName << std::endl;
        }
    }
};



#endif

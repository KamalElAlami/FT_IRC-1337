#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <sys/socket.h>
#include <iostream>

class Client
{
	private:
	public:
		bool			registered;
		int			Clientfd;
		std::string	address;
		std::string	nickName;
		std::string	userName;
		std::string	password;
		std::string	realName;
		std::string	hostName;
		Client();
		void setRegistered(bool value);
		void setClientfd(int value);
		void setAddress(std::string value);
		void setNickName(std::string value);
		void setUserName(std::string value);
		void setPassword(std::string value);
		void setRealName(std::string value);
		void setHostName(std::string value);
		/*-------------------------------------------*/
		bool getRegistered() const;
		int getClientfd() const;
		std::string getAddress() const;
		std::string getNickName() const;
		std::string getUserName() const;
		std::string getPassword() const;
		std::string getRealName() const;
		std::string getHostName() const;

		Client(Client const & src);
		std::string getNick() const;
		~Client();
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

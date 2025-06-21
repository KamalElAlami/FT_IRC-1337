#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <iostream>
#include <sys/socket.h>


class Client
{
	private:
		bool			registered;
		bool			remove_client;
		int			Clientfd;
		std::string	nickName;
		std::string	userName;
		std::string	password;
		std::string	realName;
		std::string	hostName;
		std::string	address;
	public:
		Client();
		Client(Client const & src);
		//std::string getNick() const;
		~Client();
		/*-------------------------------------------*/
		void			setClientfd(int value);
		void			setRegistered(bool value);
		void			setAddress(std::string value);
		void			setNickName(std::string value);
		void			setUserName(std::string value);
		void			setPassword(std::string value);
		void			setRealName(std::string value);
		void			setHostName(std::string value);
		void			setRemoveClient(bool value) {
			this->remove_client = value;
		};
		/*-------------------------------------------*/
		bool			getRemoveClient() const
		{
			return (this->remove_client);
		};
		std::string	getAddress() const;
		int			getClientfd() const;
		std::string	getNickName() const;
		std::string	getUserName() const;
		std::string	getPassword() const;
		std::string	getRealName() const;
		std::string	getHostName() const;
		bool		getRegistered() const;

	void sendMessage(const std::string& message) {
        std::string formatted_message = ":" + nickName + " " + message + "\r\n";
        ssize_t bytes_sent = send(Clientfd, formatted_message.c_str(), formatted_message.length(), 0);
        if (bytes_sent == -1) {
            std::cout << "Error sending message to client " << nickName << std::endl;
        }
}
};

#endif
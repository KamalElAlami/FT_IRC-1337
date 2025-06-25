#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <iostream>
#include <sys/socket.h>
#include <map>


class Client
{
	private:
		bool			registered;
		bool			remove_client;
		std::map <std::string, bool> invites;

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
		void			setInvite(std::string channel, bool stt);
		void			setRemoveClient(bool value);
		/*-------------------------------------------*/
		bool			isInvited(std::string channel);
		void 			createInvite(std::string channel, bool state);
		bool			getRemoveClient() const;
		std::string	getAddress() const;
		int			getClientfd() const;
		std::string	getNickName() const;
		std::string	getUserName() const;
		std::string	getPassword() const;
		std::string	getRealName() const;
		std::string	getHostName() const;
		bool		getRegistered() const;
		std::map <std::string, bool> getInvites();



	void sendMessage(const std::string& message) {
        std::string formatted_message = ":" + nickName + " " + message + "\r\n";
        ssize_t bytes_sent = send(Clientfd, formatted_message.c_str(), formatted_message.length(), 0);
        if (bytes_sent == -1) {
            std::cout << "Error sending message to client " << nickName << std::endl;
        }
}
};

#endif
#include "../includes/Client.hpp"
#include <cstring>
Client::Client()
{
    this->registered = false;
    this->Clientfd = -1;
    this->address = "";
    this->hostName = "";
    this->nickName = "";
    this->password = "";
    this->realName = "";
    this->userName = "";
    this->remove_client = false;
}

Client::~Client(){}

Client::Client(Client const & src)
{
	*this = src;
};
void Client::setRegistered(bool value) {
	this->registered = value;
}
void Client::setClientfd(int value) {
	this->Clientfd = value;
}
void Client::setAddress(std::string value) {
	this->address = value;
}
void Client::setNickName(std::string value) {
	this->nickName = value;
}
void Client::setUserName(std::string value) {
	this->userName = value;
}
void Client::setPassword(std::string value) {
	this->password = value;
}
void Client::setRealName(std::string value) {
	this->realName = value;
}
void Client::setHostName(std::string value) {
	this->hostName = value;
}
void Client::setInvite(std::string channel, bool stt){

	invites[channel] = stt;
}

void Client::setRemoveClient(bool value)
{
	this->remove_client = value;
}

void Client::createInvite(std::string channel, bool state)
{
	invites.insert(std::make_pair(channel, state));
}
bool Client::getRegistered() const {
	return (this->registered);
}

bool Client::isInvited(std::string channel)
{
	bool state;
	try{
		state = invites.at(channel);
		return (state);
	}
	catch (const std::out_of_range& e)
	{
		return (false);
	}
}
bool Client::getRemoveClient() const
{
	return (this->remove_client);
}
int Client::getClientfd() const {
	return (this->Clientfd);

}
std::string Client::getAddress() const {
	return (this->address);

}
std::string Client::getNickName() const {
	return (this->nickName);

}
std::string Client::getUserName() const {
	return (this->userName);

}
std::string Client::getPassword() const {
	return (this->password);

}
std::string Client::getRealName() const {
	return (this->realName);

}
std::string Client::getHostName() const {
	return (this->hostName);

}

std::map <std::string, bool> Client::getInvites()
{
	return (invites);
}

void Client::setBuffer(std::string value)
{
	this->Buffer = value;
}

void Client::CustomBuffer()
{
	ssize_t		Read;
	char			buf[1024];

	while (this->Buffer.find("\n") == std::string::npos)
	{
		memset(buf, 0, 1024);
		Read = recv(this->Clientfd, buf, sizeof(buf) -1, MSG_DONTWAIT);
		if (Read <= 0)
		{
			if (Read == -1)
				break;
			else {
				this->remove_client = true;
				return ;
			}
		}
		this->Buffer += buf;
	}
}

std::string Client::getBuffer() {
	return (Buffer);
}

//#include <iostream>
//#include <string>
//#include <vector>
//#include <unistd.h>


//std::string testref(std::string & message)
//{
//	std::string res = message.substr(0, 7);
//	message.erase(0, 7);
//	return res;
//}


//int main()
//{
//	//std::string src = "";
//	//char buf[20] = "-------------------";
//	//buf[19] = '\0';
//	//src += buf;
//	////src += "\r\n";
//	////size_t pos;
//	////if (src.find("\r\n") != std::string::npos)
//	////	std::cout << "message before : " << src << ", res : " << testref(src) << ", message after : " << src << "*-*";
//	////else if (src.find("\n") != std::string::npos)
//	////	std::cout << "--------------------------------------------------------- 2" << std::endl;
//	while(true)
//	{
//		close(5);
//	}
	
//}



#include "../includes/Server.hpp"


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
			if (Read == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
				std::cout << "we are here" << std::endl;
			//else
			//	exit (0);
		}
		this->Buffer += buf;
	}
}
std::string Client::getBuffer()
{
	return Buffer;
}

Client * Server::getClient(int clientfd)
{
	for (size_t i = 0; i < this->clients.size(); i++)
	{
		if (this->clients[i]->getClientfd() == clientfd)
			return (this->clients[i]);
	}
	return (NULL);
}

void Server::handleClientMessage(int clientFd)
{
	std::string	line;
	std::string	data;
	size_t		position;
	Client		*client;

	client = getClient(clientFd);
	client->CustomBuffer();
	data = client->getBuffer();
	while ((position = data.find("\n")) != std::string::npos && client)
	{
		line = EraseLine(data, position);
		client->setBuffer(data);
		if (!line.empty())
			this->ParseCommand(client, line);
	}
}
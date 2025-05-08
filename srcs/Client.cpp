#include "../includes/Client.hpp"

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
}

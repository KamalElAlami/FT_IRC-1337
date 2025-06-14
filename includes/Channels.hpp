#ifndef CHANNELS_HPP
#define CHANNELS_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Client.hpp"
#include <iostream>
#include <cstdlib>
#include <string>
#include <poll.h>
#include <vector>

#include <cstring>
#include <signal.h>
#include <unistd.h>

class Channel
{
    private :
        std::string name;
        std::string topic;
        bool        inviteOnly;
        bool        restrictedTopic;
        bool        enabledPass;
        int         memberLimit;
        std::string Password;
        std::vector <Client *> members;
        std::vector <Client *> ops;
        Channel();
    public :
        Channel(std::string n);
        std::string& getName(void);
        std::string& getTopic(void);
        std::vector <Client *>& getMembers(void);
        std::vector <Client *>& getOperators(void);
        bool& getInviteOnly(void);
        bool& getRestrictedTopic(void);
        bool& getEnabledPass(void);
        int&  getMemberLimit(void);
        std::string& getPassword(void);
        void addToContainer(Client* client, std::vector <Client *>& Container);
        int deleteFromContainer(Client* client, std::vector <Client *>& Container);

};
#endif
#ifndef CHANNELS_HPP
#define CHANNELS_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Client.hpp"
#include "Server.hpp"
#include <iostream>
#include <cstdlib>
#include <string>
#include <poll.h>
#include <vector>
#include <time.h>
#include <cstring>
#include <signal.h>
#include <unistd.h>


class Channel
{
    private :
        //added by soufiix
        time_t      creationTime;
        std::string topicSetBy;
        time_t      topicSetAt;
        //--------------------------
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
        //added by soufiix
        bool hasUser(int client_fd);
        std::string getTopicSetBy()const;
        time_t getTopicSetAt() const;
        bool isOperator(int client_fd)const ;
        void setTopic(std::string _topic);
        void setTopicSetBy(std::string _client);
        void setTopicSetAt(time_t _time);
        void setInviteOnly(bool status);
        void setRestrictedTopic(bool status);
        void setPassword(std::string key);
        void setEnabledPass(bool stt);
        void setMemberLimit(int n);
        void setCreationTime(time_t tt);
        // -------------------------
        time_t& getCreationTime(void);
        bool& getInviteOnly(void);
        bool& getRestrictedTopic(void);
        bool& getEnabledPass(void);
        int&  getMemberLimit(void);
        std::string& getPassword(void);
        void addToContainer(Client* client, std::vector <Client *>& Container);
        int deleteFromContainer(Client* client, std::vector <Client *>& Container);

};
#endif
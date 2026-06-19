#include "Server.hpp"
#include "utils.hpp"

void Server::handleTopic(Client &client, const std::string &param) {

    if (param.empty()) {
        sendToClient(client, ":ircserv 461 " + client.getNickname() + " TOPIC :Not enough parameters\r\n");
        return ;
    }

    std::string channelName;
    std::string newTopic;
    size_t space = param.find(' ');

    if (space == std::string::npos)
        channelName = param;
    else {
        channelName = param.substr(0, space);
        newTopic    = param.substr(space + 1);
        if (!newTopic.empty() && newTopic[0] == ':')
            newTopic = newTopic.substr(1);  // strip leading ':'
    }

    if (_channelList.find(channelName) == _channelList.end()) {
        sendToClient(client, ":ircserv 403 " + client.getNickname() + " " + channelName + IRC::toString(IRC::ERR_NOSUCHCHANNEL));
        return ;
    }
    Channel &c = _channelList[channelName];

    // viewing topic
    if (space == std::string::npos) {
        if (c.getTopic().empty())
            sendToClient(client, ":ircserv 331 " + client.getNickname() + " " + channelName + " :No topic is set\r\n");
        else {
            std::ostringstream oss;
            oss << c.getTopicTime();
            sendToClient(client, ":ircserv 332 " + client.getNickname() + " " + channelName + " :" + c.getTopic() + "\r\n");
            sendToClient(client, ":ircserv 333 " + client.getNickname() + " " + channelName + " " + c.getTopicSetter() + " " + oss.str() + "\r\n");
        }
        return ;
    }

    // setting topic
    if (c.isTopicLocked() && !c.isOperator(&client)) {
        sendToClient(client, ":ircserv 482 " + client.getNickname() + " " + channelName + IRC::toString(IRC::ERR_CHANOPRIVSNEEDED));
        return ;
    }
    c.setTopic(newTopic, client.getNickname());
    c.broadcast(":" + client.getNickname() + "!" + client.getUsername() + "@localhost TOPIC " + channelName + " :" + newTopic + "\r\n");
}

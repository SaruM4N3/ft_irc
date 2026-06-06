#include "Server.hpp"

void Server::handlePart(Client &client, const std::string &param) {
	
    std::string channelName;
    std::string reason = "Leaving"; // default msg can be replaced 

    size_t space = param.find(' ');
    if (space != std::string::npos) {
        channelName = param.substr(0, space);
        reason      = param.substr(space + 1);
    } else {
        channelName = param;
    }

	if (_channelList.find(channelName) == _channelList.end()) {
        sendToClient(client, ":ircserv 403 " + client.getNickname() + " " + channelName + " :No such channel\r\n");
        return ;
    }

    Channel &c = _channelList[channelName];

    if (!c.hasMember(&client)) {
        sendToClient(client, ":ircserv 442 " + client.getNickname() + " " + channelName + " :You're not on that channel\r\n");
        return ;
    }

    c.removeMember(&client);

	sendToClient(client, ":" + client.getNickname() + "!" + client.getUsername() + "@localhost PART " + channelName + " :" + reason + "\r\n");
    c.broadcast(":" + client.getNickname() + "!" + client.getUsername() + "@localhost PART " + channelName + " :" + reason + "\r\n");


    if (c.isEmpty())
	{
		LOG_W("Channel " + channelName + " erased");
        _channelList.erase(channelName);
	}
}

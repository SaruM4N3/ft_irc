#include "Server.hpp"

void Server::handleChannel(Client& client, const std::string& param){
	
	if (param.empty()){
        sendToClient(client, ":ircserv 461 " + client.getNickname() + " :Not enough parameters\r\n");
		return;
	}
	if (param[0] != '#' || param.size() < 2){
		sendToClient(client, ":ircserv 476 " + client.getNickname() + " " + param + " :Bad Channel Mask\r\n");
		return;
	}

	std::string	channelName;
	std::string	key;

	size_t 	space = param.find(' ');

	channelName = param.substr(0, space);
	if (space != std::string::npos)
    	key = param.substr(space + 1);

    if (_channelList.find(channelName) == _channelList.end()){
		LOG_W("Channel " + channelName + " created");
		_channelList[channelName] = Channel(channelName);
        _channelList[channelName].addMember(&client, true);
    }
    else {
		Channel &c = _channelList[channelName];
		if (c.hasMember(&client)){
			sendToClient(client, ":ircserv 443 " + client.getNickname() + " " + channelName + " :You are already a member\r\n");
			return ;
		}
		if (c.getPassword().compare("")){
			if (key.compare(c.getPassword()))
			{
				sendToClient(client, ":ircserv 475 " + client.getNickname() + " " + channelName + " :Bad channel key\r\n");
				return ;
			}
		}
		if (c.isInviteOnly()){
			if (!c.isInvited(client.getNickname())){
				sendToClient(client, ":ircserv 473 " + client.getNickname() + " " + channelName + " :Invite only channel\r\n");
				return ;
			}
		}
		if (c.getUserLimit() != -1){
			if (c.getMemberCount() >= static_cast<std::size_t>(c.getUserLimit())){
				sendToClient(client, ":ircserv 471 " + client.getNickname() + " " + channelName + " :Channel is full\r\n");
				return ;
			}
		}
		c.addMember(&client, false);
    }
	
	// store reference to avoid repeating _channelList[channelName] everywhere
    Channel &c = _channelList[channelName];

    // broadcast JOIN to everyone
    c.broadcast(":" + client.getNickname() + "!" + client.getUsername() + "@localhost JOIN :" + channelName + "\r\n");

    // send 353/366 only to the joiner 353 = RPL_NAMREPLY 366 = RPL_ENDOFNAMES
    sendToClient(client, ":ircserv 353 " + client.getNickname() + " = " + channelName + " :" + c.getMemberList() + "\r\n");
    sendToClient(client, ":ircserv 366 " + client.getNickname() + " " + channelName + " :End of NAMES list\r\n");
}

#include "Server.hpp"

void Server::handleChannel(Client& client, const std::string& param){

	std::string	channelName;
	std::string	key;

	size_t 	space = param.find(' ');

	channelName = param.substr(0, space);
	key = param.substr(space + 1);

    if (_channelList.find(channelName) == _channelList.end()){
		LOG_W("Channel " + channelName + " created");
		_channelList[channelName] = Channel(channelName);
        _channelList[channelName].addMember(&client, true);
    }
    else {
		Channel &c = _channelList[channelName];
		if (c.hasMember(&client)){
			sendToClient(client, "You are already a member of channel " + c.getName() + "\r\n");
			return ;
		}
		if (c.getPassword().compare("")){
			if (key.compare(c.getPassword()))
			{
				sendToClient(client, "Failed to join " + c.getName() + " :Invalid key\r\n");
				return ;
			}
		}
		if (c.isInviteOnly()){
			if (!c.isInvited(client.getNickname())){
				sendToClient(client, "Failed to join: Channel " + channelName + " is Invite Only\r\n");
				return ;
			}
		}
		if (c.getUserLimit() != -1){
			if (c.getMemberList().size() >= static_cast<std::size_t>(c.getUserLimit())){
				sendToClient(client, "Failed to join: Channel " + channelName + " is full\r\n");
				return ;
			}
		}
		c.addMember(&client, false);
    }
	
	// store reference to avoid repeating _channelList[channelName] everywhere
    Channel &c = _channelList[channelName];

    // broadcast JOIN to everyone
    c.broadcast(":" + client.getNickname() + "!" + client.getUsername() + "@localhost JOIN " + channelName + "\r\n");

    // send 353/366 only to the joiner 353 = RPL_NAMREPLY 366 = RPL_ENDOFNAMES
    sendToClient(client, ":ircserv 353 " + client.getNickname() + " = " + channelName + " :" + c.getMemberList() + "\r\n");
    sendToClient(client, ":ircserv 366 " + client.getNickname() + " " + channelName + " :End of NAMES list\r\n");
}

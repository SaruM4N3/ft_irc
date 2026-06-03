#include "Server.hpp"

void	Server::handleQuit(Client &client)
{
    std::string 	reason = "Quitting server";
	std::set<std::string>		channelNames;

	for (std::map<std::string, Channel>::iterator it = _channelList.begin(); it != _channelList.end(); it++)
	{
		Channel &c = it->second;
		if (c.hasMember(&client))
		{
			channelNames.insert(c.getName());
			c.removeMember(&client);
			sendToClient(client, "You left channel " + c.getName() + "\r\n");
			c.broadcast(":" + client.getNickname() + "!" + client.getUsername() + "@localhost PART " + c.getName() + " :" + reason + "\r\n");
		}
	}
	for (std::set<std::string>::const_iterator it = channelNames.begin(); it != channelNames.end(); it++)
	{
		Channel &c = _channelList[it->c_str()];
		if (c.isEmpty())
		{
			LOG_W("Channel " + c.getName() + " erased");
        	_channelList.erase(c.getName());
		}
	}
	sendToClient(client, "You disconnected from the server\r\n");
	removeClient(client.getFd());
}

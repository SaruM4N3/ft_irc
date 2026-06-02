#include "Server.hpp"

void	Server::handleKick(Client &client, const std::string &param)
{
	std::string	channelName;
	std::string	target;
	size_t 		space = param.find(' ');

	if (space == std::string::npos)
	{
		sendToClient(client, ":server ??? " + client.getNickname() + " :Failed to kick\r\n");
		return ;
	}

	channelName = param.substr(0, space);
	target = param.substr(space + 1);
	LOG_D("channelName '" + channelName + "'\ntarget '" + target + "'");

	if (channelName.empty() || target.empty())
	{
		sendToClient(client, ":server ??? " + client.getNickname() + " :Failed to kick\r\n");
		return ;
	}

	if (_channelList.find(channelName) == _channelList.end()) {
        sendToClient(client, ":ircserv 403 " + client.getNickname() + " " + channelName + " :No such channel\r\n");
        return ;
    }

	Channel &c = _channelList[channelName];
	if (!c.hasMember(&client))
	{
		sendToClient(client, ":server ??? " + client.getNickname() + " " + channelName + " :Not part of channel\r\n");
		return ;
	}
	if (!c.isOperator(&client))
	{
		sendToClient(client, "You are not operator on the channel " + channelName + "\r\n");
		return ;
	}

	Client *dest = findClient(target);
	if (!dest)
	{
		sendToClient(client, ":server 401 " + client.getNickname() + " " + target + " :No such nick\r\n");
        return ;	
	}

	c.broadcast(target + " has been kicked from channel " + channelName + " by " + client.getNickname() + "\r\n");
	sendToClient(*dest, ":" + client.getNickname() + " kick you from channel " + channelName + "\r\n");
	c.removeMember(dest);
	
	if (c.isEmpty())
	{
		LOG_W("Channel " + channelName + " erased");
        _channelList.erase(channelName);
	}
}
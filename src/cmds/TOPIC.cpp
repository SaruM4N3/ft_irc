#include "Server.hpp"

void	Server::handleTopic(Client &client, const std::string &param)
{
	std::string	channelName;
	std::string	newTopic;
	
	if (param.empty())
	{
		sendToClient(client, ":ircserv 403 " + client.getNickname() + " " + channelName + " :No such channel\r\n");
        return ;
	}

	size_t 		space = param.find(' ');
	if (space == std::string::npos)
		channelName = param;
	else
	{
		channelName = param.substr(0, space);
		newTopic = param.substr(space + 1);
	}

	if (_channelList.find(channelName) == _channelList.end()) {
        sendToClient(client, ":ircserv 403 " + client.getNickname() + " " + channelName + " :No such channel\r\n");
        return ;
    }

	Channel &c = _channelList[channelName];
	if (space == std::string::npos)
	{
		sendToClient(client, "Channel " + channelName + " topic is: " + c.getTopic() + "\r\n");
		return ;
	}

	if (c.isOperator(&client))
	{
		c.setTopic(newTopic);
		sendToClient(client, "You have changed " + channelName + " topic to: " + newTopic + "\r\n");
		c.broadcast(client.getNickname() + " has changed " + channelName + " topic to: " + newTopic + "\r\n");
		return ;
	}
	else
	{
		if (c.isTopicLocked())
		{
			if (!c.isOperator(&client))
			{
				sendToClient(client, "You are not operator on the channel " + channelName + "\r\n");
				return ;
			}
			c.setTopic(newTopic);
			sendToClient(client, "You have changed " + channelName + " topic to: " + newTopic + "\r\n");
			c.broadcast(client.getNickname() + " has changed " + channelName + " topic to: " + newTopic + "\r\n");
		}
	}
}

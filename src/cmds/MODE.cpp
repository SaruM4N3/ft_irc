#include <cstdlib>
#include "Server.hpp"

static int	findFlag(char c)
{
	char	flags[] = {'i', 't', 'k', 'o', 'l'};

	for (unsigned int i = 0; i < 5; i++)
	{
		if (c == flags[i])
			return (i);
	}
	return (-1);
}

void	Server::handleMode(Client &client, const std::string &param)
{
	std::string channelName;
	std::string flag;
	std::string arg;

	if (param.empty())
	{
		sendToClient(client, ":ircserv 403 " + client.getNickname() + " " + channelName + " :No such channel\r\n");
        return ;
	}

	size_t 	space[2];
	space[0] = param.find_first_of(' ');
	space[1] = param.find_last_of(' ');

	if (space[0] == std::string::npos || space[1] == std::string::npos)
	{
		sendToClient(client, ":ircserv 403 " + client.getNickname() + " " + channelName + " :No such channel\r\n");
        return ;
	}

	channelName = param.substr(0, space[0]);
	if (_channelList.find(channelName) == _channelList.end())
	{
        sendToClient(client, ":ircserv 403 " + client.getNickname() + " " + channelName + " :No such channel\r\n");
        return ;
    }

	Channel	&c = _channelList[channelName];
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

	if (space[1] != space[0])
	{
		flag = param.substr(space[0] + 1, space[1]);
		arg = param.substr(space[1] + 1);
	}
	else
		flag = param.substr(space[0] + 1);

	if (flag[0] != '+' && flag[0] != '-')
	{
		sendToClient(client, "Invalid flag: " + flag + "\r\n");
		return ;
	}

	int	idx = findFlag(flag[1]);
	switch (idx)
	{
	case 0:
		if (flag[0] == '+')
		{
			if (c.isInviteOnly())
				sendToClient(client, "Mode +i already set on " + c.getName() + "\r\n");
			else
			{
				c.setInviteOnly(true);
				sendToClient(client, "You have set mode +i on " + c.getName() + "\r\n");
				c.broadcast(client.getNickname() + " has set mode +i on " + c.getName() + "\r\n");
			}
		}
		else
		{
			if (!c.isInviteOnly())
				sendToClient(client, "Mode -i already set on " + c.getName() + "\r\n");
			else
			{
				c.setInviteOnly(false);
				sendToClient(client, "You have set mode -i on " + c.getName() + "\r\n");
				c.broadcast(client.getNickname() + " has set mode -i on " + c.getName() + "\r\n");
			}
		}
		break;

	case 1:
		if (flag[0] == '+')
		{
			if (c.isTopicLocked())
				sendToClient(client, "Mode +t already set on " + c.getName() + "\r\n");
			else
			{
				c.setInviteOnly(true);
				sendToClient(client, "You have set mode +t on " + c.getName() + "\r\n");
				c.broadcast(client.getNickname() + " has set mode +t on " + c.getName() + "\r\n");
			}
		}
		else
		{
			if (!c.isTopicLocked())
				sendToClient(client, "Mode -t already set on " + c.getName() + "\r\n");
			else
			{
				c.setInviteOnly(false);
				sendToClient(client, "You have set mode -t on " + c.getName() + "\r\n");
				c.broadcast(client.getNickname() + " has set mode -t on " + c.getName() + "\r\n");
			}
		}
		break;

	case 2:	
		if (flag[0] == '+')
		{
			if (!arg.c_str())
			{
				sendToClient(client, "Missing key\r\n");
				break;
			}
			c.setPassword(arg);
			sendToClient(client, "You have set channel " + c.getName() + " keyword to " + arg + "\r\n");
			c.broadcast(client.getNickname() + " has set channel " + c.getName() + " keyword to " + arg + "\r\n");
		}
		else
		{
			c.setPassword("");
			sendToClient(client, "You have removed channel " + c.getName() + " keyword\r\n");
			c.broadcast(client.getNickname() + " has removed channel " + c.getName() + " keyword\r\n");
		}
		break;

	case 3:
		if (!findClient(arg))
		{
			sendToClient(client, "Invalid Target\r\n");
			break;
		}

		if (!c.hasMember(findClient(arg)))
		{
			sendToClient(client, arg + " is not a member of channel " + c.getName() + "\r\n");
			break;
		}

		if (flag[0] == '+')
		{
			if (c.isOperator(findClient(arg)))
			{
				sendToClient(client, arg + " already has operator privilege on channel " + c.getName() + "\r\n");
				break;
			}
			c.OpPrivilege(arg, 1);
			sendToClient(client, "You have given operator privilege to " + arg + "\r\n");
			c.broadcast(client.getNickname() + " has given operator privilege to " + arg + "\r\n");
		}
		else
		{
			if (!c.isOperator(findClient(arg)))
			{
				sendToClient(client, arg + " does not have operator privilege on channel " + c.getName() + "\r\n");
				break;
			}
			c.OpPrivilege(arg, 0);
			sendToClient(client, "You have taken operator privilege from " + arg + "\r\n");
			c.broadcast(client.getNickname() + " has taken operator privilege from " + arg + "\r\n");
		}
		break;

	case 4:
		if (flag[0] == '+')
		{
			c.setUserLimit(std::atoi(arg.c_str()));
			sendToClient(client, "You have set channel " + c.getName() + " limit to " + arg + "\r\n");
			c.broadcast(client.getNickname() + " has set channel " + c.getName() + " limit to " + arg + "\r\n");
		}
		else
		{
			c.setUserLimit(-1);
			sendToClient(client, "You have removed user limit on channel " + c.getName() + "\r\n");
			c.broadcast(client.getNickname() + " has removed user limit on channel " + c.getName() + "\r\n");
		}
		break;

	default:
	{
		sendToClient(client, "Invalid flag: " + flag + "\r\n");
		break;
	}
	}	
}

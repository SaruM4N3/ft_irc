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
	std::string mode = "+";

	if (param.empty())
	{
		sendToClient(client, ":ircserv 403 " + client.getNickname() + " " + channelName + " :No such channel\r\n");
        return ;
	}

	size_t 	space[2];
	space[0] = param.find_first_of(' ');
	space[1] = param.find_last_of(' ');

	// PROBLEMS : send NO such channel at channel creation and join of existing channel if removed : invalid flag error
	// if (space[0] == std::string::npos || space[1] == std::string::npos)
	// {
	// 	sendToClient(client, ":ircserv 403 " + client.getNickname() + " " + channelName + " :No such channel\r\n");
    //     return ;
	// }

	channelName = param.substr(0, space[0]);
	if (_channelList.find(channelName) == _channelList.end())
	{
        sendToClient(client, ":ircserv 403 " + client.getNickname() + " " + channelName + " :No such channel\r\n");
        return ;
    }

	Channel	&c = _channelList[channelName];
	if (!c.hasMember(&client))
	{
		sendToClient(client, ":ircserv 442 " + client.getNickname() + " " + channelName + " :You're not on that channel\r\n");
		return ;
	}

	if (space[1] == space[0])
	{
		if (c.isInviteOnly())
			mode += "i";
		if (c.isTopicLocked())
			mode += "t";
		if (c.getUserLimit() != -1)
			mode += "l";
		sendToClient(client, ":ircserv 324 " + client.getNickname() + " " + channelName + " " + mode + "\r\n");
	}

	if (!c.isOperator(&client))
	{
		sendToClient(client, ":ircserv 482 " + client.getNickname() + " " + channelName + " :You're not channel operator\r\n");
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
		// sendToClient(client, ":ircserv 472 " + client.getNickname() + " " + channelName + " :Invalid Flag\r\n");
		return ;
	}

	int	idx = findFlag(flag[1]);
	switch (idx)
	{
	case 0:
		if (flag[0] == '+')
		{
			if (c.isInviteOnly())
				sendToClient(client, "Channel " + c.getName() + " is already Invite-only\r\n");
			else
			{
				c.setInviteOnly(true);
				sendToClient(client, ":" + client.getNickname() + "!" + client.getUsername() + "@localhost " + "MODE " + c.getName() + " +i " + arg + "\r\n");
				c.broadcastE(":" + client.getNickname() + "!" + client.getUsername() + "@localhost " + "MODE " + c.getName() + " +i " + arg + "\r\n", &client);
			}
		}
		else
		{
			if (!c.isInviteOnly())
				sendToClient(client, "Channel " + c.getName() + " is already Open to everyone\r\n");
			else
			{
				c.setInviteOnly(false);
				sendToClient(client, ":" + client.getNickname() + "!" + client.getUsername() + "@localhost " + "MODE " + c.getName() + " -i " + arg + "\r\n");
				c.broadcastE(":" + client.getNickname() + "!" + client.getUsername() + "@localhost " + "MODE " + c.getName() + " -i " + arg + "\r\n", &client);
			}
		}
		break;

	case 1:
		if (flag[0] == '+')
		{
			if (c.isTopicLocked())
				sendToClient(client, "Channel " + c.getName() + " topic is already Locked\r\n");
			else
			{
				c.setInviteOnly(true);
				sendToClient(client, ":" + client.getNickname() + "!" + client.getUsername() + "@localhost " + "MODE " + c.getName() + " +t " + arg + "\r\n");
				c.broadcastE(":" + client.getNickname() + "!" + client.getUsername() + "@localhost " + "MODE " + c.getName() + " +t " + arg + "\r\n", &client);
			}
		}
		else
		{
			if (!c.isTopicLocked())
				sendToClient(client, "Channel " + c.getName() + " topic is already Unlock\r\n");
			else
			{
				c.setInviteOnly(false);
				sendToClient(client, ":" + client.getNickname() + "!" + client.getUsername() + "@localhost " + "MODE " + c.getName() + " -t " + arg + "\r\n");
				c.broadcastE(":" + client.getNickname() + "!" + client.getUsername() + "@localhost " + "MODE " + c.getName() + " -t " + arg + "\r\n", &client);
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
			sendToClient(client, ":" + client.getNickname() + "!" + client.getUsername() + "@localhost " + "MODE " + c.getName() + " +k " + arg + "\r\n");
			c.broadcastE(":" + client.getNickname() + "!" + client.getUsername() + "@localhost " + "MODE " + c.getName() + " +k " + arg + "\r\n", &client);
		}
		else
		{
			c.setPassword("");
			sendToClient(client, ":" + client.getNickname() + "!" + client.getUsername() + "@localhost " + "MODE " + c.getName() + " -k " + arg + "\r\n");
			c.broadcastE(":" + client.getNickname() + "!" + client.getUsername() + "@localhost " + "MODE " + c.getName() + " -k " + arg + "\r\n", &client);
		}
		break;

	case 3:
		if (!findClient(arg))
		{
			sendToClient(client, ":ircserv 401 " + client.getNickname() + " " + channelName + " :No such nick\r\n");
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
			sendToClient(client, ":" + client.getNickname() + "!" + client.getUsername() + "@localhost " + "MODE " + c.getName() + " +o " + arg + "\r\n");
			c.broadcastE(":" + client.getNickname() + "!" + client.getUsername() + "@localhost " + "MODE " + c.getName() + " +o " + arg + "\r\n", &client);
		}
		else
		{
			if (!c.isOperator(findClient(arg)))
			{
				sendToClient(client, arg + " does not have operator privilege on channel " + c.getName() + "\r\n");
				break;
			}
			c.OpPrivilege(arg, 0);
			sendToClient(client, ":" + client.getNickname() + "!" + client.getUsername() + "@localhost " + "MODE " + c.getName() + " -o " + arg + "\r\n");
			c.broadcastE(":" + client.getNickname() + "!" + client.getUsername() + "@localhost " + "MODE " + c.getName() + " -o " + arg + "\r\n", &client);
		}
		break;

	case 4:
		if (flag[0] == '+')
		{
			c.setUserLimit(std::atoi(arg.c_str()));
			sendToClient(client, ":" + client.getNickname() + "!" + client.getUsername() + "@localhost " + "MODE " + c.getName() + " +l " + arg + "\r\n");
			c.broadcastE(":" + client.getNickname() + "!" + client.getUsername() + "@localhost " + "MODE " + c.getName() + " +l " + arg + "\r\n", &client);
		}
		else
		{
			c.setUserLimit(-1);
			sendToClient(client, ":" + client.getNickname() + "!" + client.getUsername() + "@localhost " + "MODE " + c.getName() + " -l " + arg + "\r\n");
			c.broadcastE(":" + client.getNickname() + "!" + client.getUsername() + "@localhost " + "MODE " + c.getName() + " -l " + arg + "\r\n", &client);
		}
		break;

	default:
	{
		sendToClient(client, ":ircserv 472 " + client.getNickname() + " " + channelName + " :Invalid Flag\r\n");
		break;
	}
	}	
}

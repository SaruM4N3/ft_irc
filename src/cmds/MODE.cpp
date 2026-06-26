/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsonie <zsonie@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 01:51:09 by zsonie            #+#    #+#             */
/*   Updated: 2026/06/27 01:51:10 by zsonie           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include "Server.hpp"
#include "utils.hpp"

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

static void	parseParam(const std::string param, std::string& channelName, std::string& flag, std::string& arg)
{
	size_t 	space[2];
	space[0] = param.find_first_of(' ');
	space[1] = param.find_last_of(' ');

	channelName = param.substr(0, space[0]);

	if (space[1] != space[0])
	{
		flag = param.substr(space[0] + 1, space[1] - space[0] - 1);
		arg = param.substr(space[1] + 1);
	}
	else
		flag = param.substr(space[0] + 1);
}

void	Server::handleMode(Client &client, const std::string &param)
{
	std::string channelName;
	std::string flag;
	std::string arg;
	std::string mode = "+";

	if (param.empty())
	{
		sendToClient(client, ":ircserv 403 " + client.getNickname() + " " + channelName + IRC::toString(IRC::ERR_NOSUCHCHANNEL));
        return ;
	}

	parseParam(param, channelName, flag, arg);

	if (_channelList.find(channelName) == _channelList.end())
	{
        sendToClient(client, ":ircserv 403 " + client.getNickname() + " " + channelName + IRC::toString(IRC::ERR_NOSUCHCHANNEL));
        return ;
    }

	Channel	&c = _channelList[channelName];
	if (!c.hasMember(&client))
	{
		sendToClient(client, ":ircserv 442 " + client.getNickname() + " " + channelName + IRC::toString(IRC::ERR_NOSUCHCHANNEL));
		return ;
	}

	if (flag == channelName || flag == "")
	{
		if (c.isInviteOnly())
			mode += "i";
		if (c.isTopicLocked())
			mode += "t";
		if (c.getPassword() != "")
			mode += "k";
		if (c.getUserLimit() != -1)
			mode += "l";
		if (c.getMemberCount() == 1)
			sendToClient(client, ":ircserv MODE " + channelName + " " + mode + "\r\n");
		sendToClient(client, ":ircserv 324 " + client.getNickname() + " " + channelName + " " + mode + "\r\n");
		return ;
	}

	if (!c.isOperator(&client))
	{
		sendToClient(client, ":ircserv 482 " + client.getNickname() + " " + channelName + IRC::toString(IRC::ERR_CHANOPRIVSNEEDED));
		return ;
	}

	if (flag[0] != '+' && flag[0] != '-')
	{
		sendToClient(client, ":ircserv 472 " + client.getNickname() + " " + channelName + " :Invalid Flag\r\n");
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
				broadcastToChannel(c, ":" + client.getNickname() + "!" + client.getUsername() + "@localhost " + "MODE " + c.getName() + " +i " + arg + "\r\n");
			}
		}
		else
		{
			if (!c.isInviteOnly())
				sendToClient(client, "Channel " + c.getName() + " is already Open to everyone\r\n");
			else
			{
				c.setInviteOnly(false);
				broadcastToChannel(c, ":" + client.getNickname() + "!" + client.getUsername() + "@localhost " + "MODE " + c.getName() + " -i " + arg + "\r\n");
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
				c.setTopicLocked(true);
				broadcastToChannel(c, ":" + client.getNickname() + "!" + client.getUsername() + "@localhost " + "MODE " + c.getName() + " +t " + arg + "\r\n");
			}
		}
		else
		{
			if (!c.isTopicLocked())
				sendToClient(client, "Channel " + c.getName() + " topic is already Unlock\r\n");
			else
			{
				c.setTopicLocked(false);
				broadcastToChannel(c, ":" + client.getNickname() + "!" + client.getUsername() + "@localhost " + "MODE " + c.getName() + " -t " + arg + "\r\n");
			}
		}
		break;

	case 2:	
		if (flag[0] == '+')
		{
			if (arg.empty())
			{
				sendToClient(client, "Missing key\r\n");
				break;
			}
			c.setPassword(arg);
			broadcastToChannel(c, ":" + client.getNickname() + "!" + client.getUsername() + "@localhost " + "MODE " + c.getName() + " +k " + arg + "\r\n");
		}
		else
		{
			c.setPassword("");
			broadcastToChannel(c, ":" + client.getNickname() + "!" + client.getUsername() + "@localhost " + "MODE " + c.getName() + " -k " + arg + "\r\n");
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
			broadcastToChannel(c, ":" + client.getNickname() + "!" + client.getUsername() + "@localhost " + "MODE " + c.getName() + " +o " + arg + "\r\n");
		}
		else
		{
			if (!c.isOperator(findClient(arg)))
			{
				sendToClient(client, arg + " does not have operator privilege on channel " + c.getName() + "\r\n");
				break;
			}
			c.OpPrivilege(arg, 0);
			broadcastToChannel(c, ":" + client.getNickname() + "!" + client.getUsername() + "@localhost " + "MODE " + c.getName() + " -o " + arg + "\r\n");
		}
		break;

	case 4:
		if (flag[0] == '+')
		{
			c.setUserLimit(std::atoi(arg.c_str()));
			broadcastToChannel(c, ":" + client.getNickname() + "!" + client.getUsername() + "@localhost " + "MODE " + c.getName() + " +l " + arg + "\r\n");
		}
		else
		{
			c.setUserLimit(-1);
			broadcastToChannel(c, ":" + client.getNickname() + "!" + client.getUsername() + "@localhost " + "MODE " + c.getName() + " -l " + arg + "\r\n");
		}
		break;

	default:
	{
		sendToClient(client, ":ircserv 472 " + client.getNickname() + " " + channelName + " :Invalid Flag\r\n");
		break;
	}
	}	
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BOT.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaamonch <vaamonch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 02:29:49 by vaamonch          #+#    #+#             */
/*   Updated: 2026/06/27 03:37:30 by vaamonch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "utils.hpp"

void	Server::handleBOT(Client &client, const std::string &param)
{
	std::string channelName;
	size_t space = param.find(' ');
	
	if (param.empty())
	{
		sendToClient(client, ":ircserv 403 " + client.getNickname() + " " + param + IRC::toString(IRC::ERR_NOSUCHCHANNEL));
        return ;
	}

    if (space != std::string::npos)
        channelName = param.substr(0, space);
	else
        channelName = param;

	if (_channelList.find(param) == _channelList.end())
	{
        sendToClient(client, ":ircserv 403 " + client.getNickname() + " " + channelName + IRC::toString(IRC::ERR_NOSUCHCHANNEL));
        return ;
    }

	Channel &c = _channelList[channelName];

	if (!c.hasMember(&client))
	{
		sendToClient(client, ":ircserv 442 " + client.getNickname() + " " + channelName + IRC::toString(IRC::ERR_NOSUCHCHANNEL));
		return ;
	}

	if (!c.isOperator(&client))
	{
		sendToClient(client, ":ircserv 482 " + client.getNickname() + " " + channelName + IRC::toString(IRC::ERR_CHANOPRIVSNEEDED));
		return ;
	}
	
	if (c.getBOTStatus() == false)
	{
		c.setBOTStatus(true);
		broadcastToChannel(c, ":BOT!BOT@localhost JOIN :" + channelName + "\r\n");
	}
}
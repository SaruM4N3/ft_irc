/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaamonch <vaamonch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 01:51:04 by zsonie            #+#    #+#             */
/*   Updated: 2026/06/27 03:38:01 by vaamonch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "utils.hpp"

void	Server::handleKick(Client &client, const std::string &param)
{
	std::string target;
    std::string reason;

    size_t space = param.find(' ');

	std::string channelName = param.substr(0, space);
	std::string rest = param.substr(space + 1);
	
	// check for reason after target
	size_t reasonPos = rest.find(' ');
	if (reasonPos != std::string::npos) {
	    target = rest.substr(0, reasonPos);
	    reason = rest.substr(reasonPos + 1);
	    if (!reason.empty() && reason[0] == ':')
	        reason = reason.substr(1);
	    if (reason.empty())
	        reason = "Kicked";
	} else {
	    target = rest;
	    reason = "Kicked";  // default
	}
	LOG_D("channelName '" + channelName + "'\ntarget '" + target + "'");

	if (channelName.empty() || target.empty() || space == std::string::npos)
	{
		sendToClient(client, ":ircserv 461 " + client.getNickname() + IRC::toString(IRC::ERR_NEEDMOREPARAMS));
		return ;
	}

	if (_channelList.find(channelName) == _channelList.end()) {
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

	if (target == "BOT")
	{
		c.setBOTStatus(false);
		broadcastToChannel(c, ":" + client.getNickname() + "!" + client.getUsername() + "@localhost KICK " + channelName + " BOT :OVERRIDE / You are a machine" + "\r\n");
		return ;
	}
	Client *dest = findClient(target);
	if (!dest)
	{
		sendToClient(client, ":ircserv 441 " + client.getNickname() + " " + target + " " + channelName + IRC::toString(IRC::ERR_USERNOTINCHANNEL));
        return ;	
	}

	broadcastToChannel(c, ":" + client.getNickname() + "!" + client.getUsername() + "@localhost KICK " + channelName + " " + target + " :" + reason + "\r\n");
	c.removeMember(dest);
	
	if (c.isEmpty())
	{
		LOG_W("Channel " + channelName + " erased");
        _channelList.erase(channelName);
	}
}
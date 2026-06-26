/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PART.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsonie <zsonie@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 01:51:15 by zsonie            #+#    #+#             */
/*   Updated: 2026/06/27 01:51:16 by zsonie           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "utils.hpp"

void Server::handlePart(Client &client, const std::string &param) {
	
    std::string channelName;
    std::string reason = "Leaving"; // default msg can be replaced 

    size_t space = param.find(' ');
    if (space != std::string::npos) {
        channelName = param.substr(0, space);
        reason      = param.substr(space + 1);
    } else {
        channelName = param;
    }

	if (_channelList.find(channelName) == _channelList.end()) {
        sendToClient(client, ":ircserv 403 " + client.getNickname() + " " + channelName + IRC::toString(IRC::ERR_NOSUCHCHANNEL));
        return ;
    }

    Channel &c = _channelList[channelName];

    if (!c.hasMember(&client)) {
        sendToClient(client, ":ircserv 442 " + client.getNickname() + " " + channelName + IRC::toString(IRC::ERR_NOSUCHCHANNEL));
        return ;
    }

    c.removeMember(&client);

	sendToClient(client, ":" + client.getNickname() + "!" + client.getUsername() + "@localhost PART " + channelName + " :" + reason + "\r\n");
    broadcastToChannel(c, ":" + client.getNickname() + "!" + client.getUsername() + "@localhost PART " + channelName + " :" + reason + "\r\n");


    if (c.isEmpty())
	{
		LOG_W("Channel " + channelName + " erased");
        _channelList.erase(channelName);
	}
}

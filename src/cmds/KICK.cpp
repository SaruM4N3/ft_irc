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
	    reason = rest.substr(reasonPos + 2);  // skip ' :'
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
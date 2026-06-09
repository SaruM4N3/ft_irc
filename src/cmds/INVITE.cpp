#include "Server.hpp"
#include "utils.hpp"

void	Server::handleInvite(Client &client, const std::string &param)
{
	std::string	channelName;
	std::string	target;
	size_t space = param.find(' ');

	if (space == std::string::npos)
	{
		sendToClient(client, ":ircserv 461 " + client.getNickname() + IRC::toString(IRC::ERR_NEEDMOREPARAMS));
		return ;
	}

	target      = param.substr(0, space);
	channelName = param.substr(space + 1);
	LOG_D("channelName '" + channelName + "'\ntarget '" + target + "'");

	if (channelName.empty() || target.empty())
	{
		sendToClient(client, ":ircserv 461 " + client.getNickname() + IRC::toString(IRC::ERR_NEEDMOREPARAMS));
		return ;
	}

	if (_channelList.find(channelName) == _channelList.end())
	{
        sendToClient(client, ":ircserv 403 " + client.getNickname() + IRC::toString(IRC::ERR_NOSUCHCHANNEL));
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
		sendToClient(client, ":ircserv 482 " + client.getNickname() + " " + target + IRC::toString(IRC::ERR_CHANOPRIVSNEEDED));
		return ;
	}

	Client *dest = findClient(target);
	if (!dest)
	{
		sendToClient(client, ":ircserv 401 " + client.getNickname() + " " + target + IRC::toString(IRC::ERR_NOSUCHNICK));
        return ;	
	}

	c.addInvitation(target);
	// optionnal broadcast invitation notice
	// c.broadcast(":ircserv NOTICE " + channelName + " :" + client.getNickname() + " invited " + target + " to the channel\r\n");
	sendToClient(client, ":ircserv 341 " + client.getNickname() + " " + target + " " + channelName + "\r\n");
	sendToClient(*dest, ":" + client.getNickname() + "!" + client.getUsername() + "@localhost INVITE " + target + " :" + channelName + "\r\n");
}

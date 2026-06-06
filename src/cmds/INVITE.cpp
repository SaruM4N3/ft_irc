#include "Server.hpp"

/*
// 341  RPL_INVITING		 :		
// 461  ERR_NEEDMOREPARAMS   :Not enough parameters
// 401  ERR_NOSUCHNICK       :No such nick
// 403  ERR_NOSUCHCHANNEL    :No such channel
// 442  ERR_NOTONCHANNEL     :You're not on that channel
// 482  ERR_CHANOPRIVSNEEDED :You're not channel operator
// 443  ERR_USERONCHANNEL    :User already on channel
*/

void	Server::handleInvite(Client &client, const std::string &param)
{
	std::string	channelName;
	std::string	target;
	size_t space = param.find(' ');

	if (space == std::string::npos)
	{
		// 461  ERR_NEEDMOREPARAMS   :Not enough parameters
		sendToClient(client, ":ircserver 461 " + client.getNickname() + " :Not enough parameters\r\n");
		return ;
	}

	target      = param.substr(0, space);
	channelName = param.substr(space + 1);
	LOG_D("channelName '" + channelName + "'\ntarget '" + target + "'");

	if (channelName.empty() || target.empty())
	{
		// 461  ERR_NEEDMOREPARAMS   :Not enough parameters
		sendToClient(client, ":ircserver 461 " + client.getNickname() + " :Not enough parameters\r\n");
		return ;
	}

	if (_channelList.find(channelName) == _channelList.end())
	{
		// 403  ERR_NOSUCHCHANNEL    :No such channel
        sendToClient(client, ":ircserv 403 " + client.getNickname() + " " + channelName + " :No such channel\r\n");
        return ;
    }

	Channel &c = _channelList[channelName];
	if (!c.hasMember(&client))
	{
		// 442  ERR_NOTONCHANNEL     :You're not on that channel
		sendToClient(client, ":ircserver 442 " + client.getNickname() + " " + channelName + " :You're not on that channel\r\n");
		return ;
	}
	if (!c.isOperator(&client))
	{
		// 482  ERR_CHANOPRIVSNEEDED :You're not channel operator
		sendToClient(client, ":ircserver 482 " + client.getNickname() + " " + target + " :You're not channel operator\r\n");
		return ;
	}

	Client *dest = findClient(target);
	if (!dest)
	{
		sendToClient(client, ":ircserver 401 " + client.getNickname() + " " + target + " :No such nick\r\n");
        return ;	
	}

	c.addInvitation(target);
	// optionnal broadcast invitation notice
	// c.broadcast(":ircserv NOTICE " + channelName + " :" + client.getNickname() + " invited " + target + " to the channel\r\n");
	sendToClient(client, ":ircserv 341 " + client.getNickname() + " " + target + " " + channelName + "\r\n");
	sendToClient(*dest, ":" + client.getNickname() + "!" + client.getUsername() + "@localhost INVITE " + target + " :" + channelName + "\r\n");
}

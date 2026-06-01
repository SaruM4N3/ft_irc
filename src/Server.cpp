/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaamonch <vaamonch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 17:51:15 by zsonie            #+#    #+#             */
/*   Updated: 2026/06/01 09:14:06 by vaamonch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include <unistd.h>
#include <cstring>
#include <string>
#include <fcntl.h>
#include <cstdlib>
#include "Debug.hpp"

//--------------------Constructor/Destructor---------------------------
Server::Server(int port, const std::string& password)
	: _port(port), _password(password), _serverFd(-1), _epFd(-1) {
	LOG_I("Constructor: Server");
	LOG_D("Port: " + toString(_port) + " Password: " + _password);
	init();
}

Server::~Server() {
	LOG_I("Destructor: Server");
	// Close every existing client
	for (std::map<int, Client*>::iterator it = _clientMap.begin();
		 it != _clientMap.end(); ++it) {
		close(it->first);
		delete it->second;
	}
	// Only close when exist (in case error on initialisation)
	if (_serverFd != -1) close(_serverFd);
	if (_epFd != -1) close(_epFd);
}

//---------------------------Methods-------------------------------------

void Server::init() {
	LOG_I("Server initialisation");
	// Socketcreation
	_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverFd == -1)
		throw std::runtime_error("socket: " +
								 std::string(std::strerror(errno)));

	// Setoptions
	int opt = 1;
	if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) ==
		-1)
		throw std::runtime_error("setsockopt: " +
								 std::string(std::strerror(errno)));

	// Bind
	struct sockaddr_in addr;
	socklen_t socklength = sizeof(addr);
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(_port);
	if (bind(_serverFd, (struct sockaddr*)&addr, socklength) == -1)
		throw std::runtime_error("bind: " + std::string(std::strerror(errno)));

	// Listen
	if (listen(_serverFd, SOMAXCONN) == -1)
		throw std::runtime_error("listen: " +
								 std::string(std::strerror(errno)));
	setNonBlocking(_serverFd);

	// Epoll instantiation
	_epFd = epoll_create1(0);
	if (_epFd == -1)
		throw std::runtime_error("epoll: " + std::string(std::strerror(errno)));
	epollAdd(_serverFd, EPOLLIN);
}

void Server::update() {
	struct epoll_event events[MAX_EVENTS];

	LOG_I("Server is running");
	while (true) {
		int n = epoll_wait(_epFd, events, MAX_EVENTS, -1);
		if (n == -1)
			throw std::runtime_error("epoll_wait: " +
									 std::string(strerror(errno)));

		for (int i = 0; i < n; i++) {
			int fd = events[i].data.fd;

			if (events[i].events & (EPOLLERR | EPOLLHUP))
				removeClient(fd);
			else if (fd == _serverFd)
				acceptClient();
			else if (events[i].events & EPOLLIN)
				handleClient(fd);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////
//----------------------EPOLL---------------------------------------------------/
/////////////////////////////////////////////////////////////////////////////////

void Server::epollAdd(int fd, uint32_t events) {
	struct epoll_event ev;
	ev.events = events;
	ev.data.fd = fd;
	if (epoll_ctl(_epFd, EPOLL_CTL_ADD, fd, &ev) == -1)
		throw std::runtime_error("epoll_ctl ADD: " +
								 std::string(strerror(errno)));
}

void Server::epollDel(int fd) {
	if (epoll_ctl(_epFd, EPOLL_CTL_DEL, fd, NULL) == -1)
		throw std::runtime_error("epoll_ctl DEL: " +
								 std::string(strerror(errno)));
}

/////////////////////////////////////////////////////////////////////////////////
//----------------------CLIENT--------------------------------------------------/
/////////////////////////////////////////////////////////////////////////////////

void Server::acceptClient() {
	struct sockaddr_in clientAddr;
	socklen_t len = sizeof(clientAddr);

	int clientFd = accept(_serverFd, (struct sockaddr*)&clientAddr, &len);
	if (clientFd == -1) return;
	setNonBlocking(clientFd);
	epollAdd(clientFd, EPOLLIN);
	_clientMap[clientFd] = new Client(clientFd, clientAddr);
	LOG_W("New client connected: fd[" + toString(clientFd) + "]");
}

void Server::handleClient(int fd) {
	char buf[4096];
	int bytes = recv(fd, buf, sizeof(buf), 0);
	if (bytes <= 0) {
		removeClient(fd);
		return;
	}
	Client* client = _clientMap[fd];
	client->appendToBuffer(std::string(buf, bytes));

	while (_clientMap.count(fd) && client->isMessageReceived())
		processMessage(*client, client->extractMessage());
	LOG_D("Received data from fd[" + toString(fd) + "]");
}

void Server::removeClient(int fd) {
	epollDel(fd);
	close(fd);
	delete _clientMap[fd];
	_clientMap.erase(fd);
	LOG_D("Client disconnected: fd[" + toString(fd) + "]");
}

/////////////////////////////////////////////////////////////////////////////////
//----------------------CMDS----------------------------------------------------/
/////////////////////////////////////////////////////////////////////////////////

void Server::processMessage(Client& client, const std::string& msg) {
	if (msg.empty()) return;

	// cmd extract
	std::string cmd = msg.substr(0, msg.find(' '));

	// param extract
	std::string params;
	size_t spacePos = msg.find(' ');
	if (spacePos != std::string::npos) params = msg.substr(spacePos + 1);

	// handleCmds
	LOG_I("CMD=[" + cmd + "] PARAMS=[" + params + "]");
	if (cmd == "PASS")
		handlePass(client, params);
	else if (!client.isAuthenticated()) {
		sendToClient(client,
					 "Error: You must send PASS before any other actions\r\n");
		return;
	} else if (cmd == "NICK")
		handleNickname(client, params);
	else if (cmd == "USER")
		handleUsername(client, params);
	else if (cmd == "JOIN")
		handleChannel(client, params);
	else if (cmd == "PRIVMSG")
		handleCom(client, params);
	else if (cmd == "PART")
		handlePart(client, params);
	else if (cmd == "INVITE")
		handleInvite(client, params);
	else if (cmd == "KICK")
		handleKick(client, params);
	else if (cmd == "TOPIC")
		handleTopic(client, params);
	else if (cmd == "MODE")
		handleMode(client, params);
	else if (cmd == "QUIT")
		handleQuit(client);
}

void Server::handlePass(Client& client, const std::string& param) {
	if (param == _password) {
		client.setAuthenticated(true);
		LOG_W("Client on fd[" + toString(client.getFd()) +
			  "]: Connected succesfully");
	} else {
		sendToClient(client, "Wrong password\r\n");
		LOG_W("Client on fd[" + toString(client.getFd()) +
			  "]: Failed to connect");
		LOG_E("Removing client");
		removeClient(client.getFd());
		return;
	}
}

void Server::handleNickname(Client& client, const std::string& param) {
	client.setNickname(param);
	if (!client.getUsername().empty()) {
		client.setRegistered(true);
		sendToClient(client, ":ircserv 001 " + client.getNickname() +
								 " :Welcome to the IRC server\r\n");
	}
}

void Server::handleUsername(Client& client, const std::string& param) {
	client.setUsername(param);
	if (!client.getNickname().empty()) {
		client.setRegistered(true);
		sendToClient(client, ":ircserv 001 " + client.getNickname() +
								 " :Welcome to the IRC server\r\n");
	}
}

/* JOIN cmd*/
void Server::handleChannel(Client& client, const std::string& param){


    if (_channelList.find(param) == _channelList.end()){
		LOG_W("Channel " + param + " created");
		_channelList[param] = Channel(param);
        _channelList[param].addMember(&client, true);
    }
    else {
		Channel &c = _channelList[param];
		if (c.hasMember(&client)){
			sendToClient(client, "You are already a member of channel " + c.getName() + "\r\n");
			return ;
		}
		if (!c.isInviteOnly())
			c.addMember(&client, false);
		else {
			if (c.isInvited(client.getNickname()))
				c.addMember(&client, false);
			else {
				sendToClient(client, "Failed to join: Channel " + param + " is InviteOnly\r\n");
				return ;
			}
		}
    }
	
	// store reference to avoid repeating _channelList[param] everywhere
    Channel &c = _channelList[param];

    // broadcast JOIN to everyone
    c.broadcast(":" + client.getNickname() + "!" + client.getUsername() + "@localhost JOIN " + param + "\r\n");

    // send 353/366 only to the joiner 353 = RPL_NAMREPLY 366 = RPL_ENDOFNAMES
    sendToClient(client, ":ircserv 353 " + client.getNickname() + " = " + param + " :" + c.getMemberList() + "\r\n");
    sendToClient(client, ":ircserv 366 " + client.getNickname() + " " + param + " :End of NAMES list\r\n");
}

/* PRIVMSG cmd*/
void Server::handleCom(Client &client, const std::string &param) {
	
	std::string target;
	std::string msg;
	
	size_t pos = param.find(' ');
	if ( pos == std::string::npos){
		sendToClient(client, ":server 411 " + client.getNickname() + " :No text to send\r\n");
		return ;
	}
	
	target 	= param.substr(0, pos);
	msg 	= param.substr(pos + 1);

	if ( target.empty() || msg.empty()){
		sendToClient(client, ":server 411 " + client.getNickname() + " :No text to send\r\n");
		return ;
	}
	
	if (target[0] == '#')
		_channelList[target].broadcast(":" + client.getNickname() + " PRIVMSG " + target + " :" + msg + "\r\n");
	else {
        Client *dest = findClient(target);
        if (!dest) {
            sendToClient(client, ":server 401 " + client.getNickname() + " " + target + " :No such nick\r\n");
            return ;
        }
        sendToClient(*dest, ":" + client.getNickname() + " PRIVMSG " + target + " :" + msg + "\r\n");
		return;
    }
}

/*PART cmd*/
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
        sendToClient(client, ":ircserv 403 " + client.getNickname() + " " + channelName + " :No such channel\r\n");
        return ;
    }

    Channel &c = _channelList[channelName];

    if (!c.hasMember(&client)) {
        sendToClient(client, ":ircserv 442 " + client.getNickname() + " " + channelName + " :You're not on that channel\r\n");
        return ;
    }

    c.removeMember(&client);

	sendToClient(client, "You left channel " + c.getName() + "\r\n");
    c.broadcast(":" + client.getNickname() + "!" + client.getUsername() + "@localhost PART " + channelName + " :" + reason + "\r\n");


    if (c.isEmpty())
	{
		LOG_W("Channel " + channelName + " erased");
        _channelList.erase(channelName);
	}
}

/*INVITE cmd*/
void	Server::handleInvite(Client &client, const std::string &param)
{
	std::string	channelName;
	std::string	target;
	size_t space = param.find(' ');

	if (space == std::string::npos)
	{
		sendToClient(client, ":server ??? " + client.getNickname() + " :Failed to send invite\r\n");
		return ;
	}

	channelName = param.substr(0, space);
	target = param.substr(space + 1);
	LOG_D("channelName '" + channelName + "'\ntarget '" + target + "'");

	if (channelName.empty() || target.empty())
	{
		sendToClient(client, ":server ??? " + client.getNickname() + " :Failed to send invite\r\n");
		return ;
	}

	if (_channelList.find(channelName) == _channelList.end())
	{
        sendToClient(client, ":ircserv 403 " + client.getNickname() + " " + channelName + " :No such channel\r\n");
        return ;
    }

	Channel &c = _channelList[channelName];
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

	Client *dest = findClient(target);
	if (!dest)
	{
		sendToClient(client, ":server 401 " + client.getNickname() + " " + target + " :No such nick\r\n");
        return ;	
	}

	c.addInvitation(target);
	c.broadcast(target + " has been invited to " + channelName + " by " + client.getNickname() + "\r\n");
	sendToClient(*dest, ":" + client.getNickname() + " Invited you to " + channelName + "\r\n");
}

/*KICK cmd*/
void	Server::handleKick(Client &client, const std::string &param)
{
	std::string	channelName;
	std::string	target;
	size_t 		space = param.find(' ');

	if (space == std::string::npos)
	{
		sendToClient(client, ":server ??? " + client.getNickname() + " :Failed to kick\r\n");
		return ;
	}

	channelName = param.substr(0, space);
	target = param.substr(space + 1);
	LOG_D("channelName '" + channelName + "'\ntarget '" + target + "'");

	if (channelName.empty() || target.empty())
	{
		sendToClient(client, ":server ??? " + client.getNickname() + " :Failed to kick\r\n");
		return ;
	}

	if (_channelList.find(channelName) == _channelList.end()) {
        sendToClient(client, ":ircserv 403 " + client.getNickname() + " " + channelName + " :No such channel\r\n");
        return ;
    }

	Channel &c = _channelList[channelName];
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

	Client *dest = findClient(target);
	if (!dest)
	{
		sendToClient(client, ":server 401 " + client.getNickname() + " " + target + " :No such nick\r\n");
        return ;	
	}

	c.broadcast(target + " has been kicked from channel " + channelName + " by " + client.getNickname() + "\r\n");
	sendToClient(*dest, ":" + client.getNickname() + " kick you from channel " + channelName + "\r\n");
	c.removeMember(dest);
	
	if (c.isEmpty())
	{
		LOG_W("Channel " + channelName + " erased");
        _channelList.erase(channelName);
	}
}

/*TOPIC cmd*/
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

/*MODE cmd*/

int	findFlag(char c)
{
	char	flags[] = {'i', 't', 'k', 'o', 'l'};

	for (unsigned int i = 0; i < 5; i++)
	{
		std::string msg = "  flags[i]:  "; 	//
		msg[0] = c;							//
		msg[12] = flags[i]; 				//
	
		LOG_I(msg);
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
				sendToClient(client, "Channel " + c.getName() + " is already Invite-only\r\n");
			else
			{
				c.setInviteOnly(true);
				sendToClient(client, "You have changed " + c.getName() + " to Invite-only\r\n");
				c.broadcast(client.getNickname() + " has changed " + c.getName() + " to Invite-only\r\n");
			}
		}
		else
		{
			if (!c.isInviteOnly())
				sendToClient(client, "Channel " + c.getName() + " is already Open to everyone\r\n");
			else
			{
				c.setInviteOnly(false);
				sendToClient(client, "You have changed " + c.getName() + " to Open to everyone\r\n");
				c.broadcast(client.getNickname() + " has changed " + c.getName() + " to Open to everyone\r\n");
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
				sendToClient(client, "You have locked " + c.getName() + " topic\r\n");
				c.broadcast(client.getNickname() + " has locked " + c.getName() + " topic\r\n");
			}
		}
		else
		{
			if (!c.isTopicLocked())
				sendToClient(client, "Channel " + c.getName() + " topic is already Unlock\r\n");
			else
			{
				c.setInviteOnly(false);
				sendToClient(client, "You have unlocked " + c.getName() + " topic\r\n");
				c.broadcast(client.getNickname() + " has unlocked " + c.getName() + " topic\r\n");
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
			sendToClient(client, "You have changed " + c.getName() + " key\r\n");
			c.broadcast(client.getNickname() + " has changed " + c.getName() + " key\r\n");
		}
		else
		{
			c.setPassword("");
			sendToClient(client, "You have removed " + c.getName() + " key\r\n");
			c.broadcast(client.getNickname() + " has removed " + c.getName() + " key\r\n");
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
			sendToClient(client, "You have set a user limit of " + arg + " on " + c.getName() + "\r\n");
			c.broadcast(client.getNickname() + " has set a user limit of " + arg + " on " + c.getName() + "\r\n");
		}
		else
		{
			c.setUserLimit(-1);
			sendToClient(client, "You have removed user limit from " + c.getName() + "\r\n");
			c.broadcast(client.getNickname() + " has removed user limit from " + c.getName() + "\r\n");
		}
		break;

	default:
	{
		sendToClient(client, "Invalid flag: " + flag + "\r\n");
		break;
	}
	}	
}


/*QUIT cmd*/
void	Server::handleQuit(Client &client)
{
    std::string 	reason = "Quitting server";
	std::set<std::string>		channelNames;

	for (std::map<std::string, Channel>::iterator it = _channelList.begin(); it != _channelList.end(); it++)
	{
		Channel &c = it->second;
		if (c.hasMember(&client))
		{
			channelNames.insert(c.getName());
			c.removeMember(&client);
			sendToClient(client, "You left channel " + c.getName() + "\r\n");
			c.broadcast(":" + client.getNickname() + "!" + client.getUsername() + "@localhost PART " + c.getName() + " :" + reason + "\r\n");
		}
	}
	for (std::set<std::string>::const_iterator it = channelNames.begin(); it != channelNames.end(); it++)
	{
		Channel &c = _channelList[it->c_str()];
		if (c.isEmpty())
		{
			LOG_W("Channel " + c.getName() + " erased");
        	_channelList.erase(c.getName());
		}
	}
	sendToClient(client, "You disconnected from the server (Not really, I'm still working on it)\r\n");
}


/////////////////////////////////////////////////////////////////////////////////
//----------------------UTILS---------------------------------------------------/
/////////////////////////////////////////////////////////////////////////////////

Client* Server::findClient(const std::string &nick) {
    for (std::map<int, Client*>::iterator it = _clientMap.begin();
         it != _clientMap.end(); it++) {
        if (it->second->getNickname() == nick)
            return it->second;
    }
    return NULL;
}

void Server::setNonBlocking(int fd) {
	LOG_D("Server: setNonblocking called");
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
		throw std::runtime_error("fcntl F_GETFL: " +
								 std::string(strerror(errno)));
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
		throw std::runtime_error("fcntl F_SETFL: " +
								 std::string(strerror(errno)));
}

void Server::sendToClient(Client& client, const std::string& msg) {
	send(client.getFd(), msg.c_str(), msg.size(), 0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erbuffet <erbuffet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 17:51:15 by zsonie            #+#    #+#             */
/*   Updated: 2026/05/25 06:17:16 by erbuffet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include <unistd.h>
#include <cstring>
#include <string>
#include <fcntl.h>
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
        _channelList[param] = Channel(param);
        _channelList[param].addMember(&client, true);
    }
    else {
        _channelList[param].addMember(&client, false);
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

    c.broadcast(":" + client.getNickname() + "!" + client.getUsername() + "@localhost PART " + channelName + " :" + reason + "\r\n");

    c.removeMember(&client);

    if (c.isEmpty())
        _channelList.erase(channelName);
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
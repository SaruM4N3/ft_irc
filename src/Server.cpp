/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaamonch <vaamonch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 06:17:53 by zsonie            #+#    #+#             */
/*   Updated: 2026/06/27 03:02:31 by vaamonch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "utils.hpp"
#include <unistd.h>
#include <cstring>
#include <string>
#include <fcntl.h>
#include <csignal>
#include "Debug.hpp"

namespace {
	volatile sig_atomic_t g_running = 1;
	void handleSigint(int) { g_running = 0; }
}

//--------------------Constructor/Destructor---------------------------
Server::Server(int port, const std::string& password)
	: _port(port), _password(password), _serverFd(-1), _epFd(-1) {
	LOG_I("Constructor: Server");
	LOG_D("Port: " + toString(_port) + " Password: " + _password);
	_cmdHandlers["PASS"]    = &Server::handlePass;
    _cmdHandlers["NICK"]    = &Server::handleNickname;
    _cmdHandlers["USER"]    = &Server::handleUsername;
    _cmdHandlers["JOIN"]    = &Server::handleChannel;
    _cmdHandlers["PRIVMSG"] = &Server::handleCom;
    _cmdHandlers["PART"]    = &Server::handlePart;
    _cmdHandlers["INVITE"]  = &Server::handleInvite;
    _cmdHandlers["KICK"]    = &Server::handleKick;
    _cmdHandlers["TOPIC"]   = &Server::handleTopic;
    _cmdHandlers["MODE"]    = &Server::handleMode;
	_cmdHandlers["QUIT"]    = &Server::handleQuitWrapper;
	_cmdHandlers["BOT"]    	= &Server::handleBOT;
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
	struct sockaddr_in addr = {};
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

	signal(SIGINT, handleSigint);
}
void Server::update() {

	struct epoll_event events[MAX_EVENTS];

	LOG_I("Server is running");
	while (g_running) {
		int n = epoll_wait(_epFd, events, MAX_EVENTS, -1);
		if (n == -1) {
			if (errno == EINTR)
				continue;
			throw std::runtime_error("epoll_wait: " +
									 std::string(strerror(errno)));
		}
		for (int i = 0; i < n; i++) {
			int fd = events[i].data.fd;
	
			if (events[i].events & (EPOLLERR | EPOLLHUP))
			{
				removeClient(fd);
				continue;
			}
			if (fd == _serverFd)
			{
				acceptClient();
				continue;	
			}
			if (events[i].events & EPOLLIN)
				handleClient(fd);
			if (events[i].events & EPOLLOUT && _clientMap.count(fd))
				flushClient(*_clientMap[fd]);
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

void Server::epollMod(int fd, uint32_t events) {
	struct epoll_event ev;
	ev.events = events;
	ev.data.fd = fd;
	if (epoll_ctl(_epFd, EPOLL_CTL_MOD, fd, &ev) == -1)
		throw std::runtime_error("epoll_ctl MOD: " +
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

	while (_clientMap.count(fd)) {
		if (client->isMessageReceived()) {
			if (client->nextLineFits(MAX_LINE_LEN)) {
				processMessage(*client, client->extractMessage());
				continue;
			}
			if (client->trySplitOversizedMessage(MAX_LINE_LEN))
				continue;
			processMessage(*client, client->extractMessage());
			continue;
		}
		if (client->inBufferSize() <= MAX_LINE_LEN)
			break;
		if (client->trySplitOversizedMessage(MAX_LINE_LEN))
			continue;
		LOG_W("Client fd[" + toString(fd) + "] sent an oversized line, disconnecting");
		sendToClient(*client, "ERROR :Closing Link: line too long\r\n");
		removeClient(fd);
		return;
	}
	LOG_D("Received data from fd[" + toString(fd) + "]");
}

// Flushes _outBuffer. If data remains (!_outBuffer.empty()), enables EPOLLOUT so epoll
// notifies us when the socket is ready to write again.
void Server::flushClient(Client& client)
{
	client.flushOutBuffer();
	if (client.isWaiting())
		epollMod(client.getFd(), EPOLLIN | EPOLLOUT);
	else
		epollMod(client.getFd(), EPOLLIN);
}

void Server::removeClient(int fd) {
	epollDel(fd);
	close(fd);
	delete _clientMap[fd];
	_clientMap.erase(fd);
	LOG_W("Client disconnected: fd[" + toString(fd) + "]");
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

	LOG_I("CMD=[" + cmd + "] PARAMS=[" + params + "]");

	if (!client.isAuthenticated() && cmd != "PASS") {
		sendToClient(client,
					 "Error: You must send PASS before any other actions\r\n");
		return;
	}

	std::map<std::string, CmdHandler>::iterator it = _cmdHandlers.find(cmd);
	if (it != _cmdHandlers.end()) {
		(this->*(it->second))(client, params);
	}
}

void Server::handleQuitWrapper(Client &client, const std::string &param) {
	(void)param;
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
	if (param != client.getNickname() && (findClient(param) || param == "BOT")) {
		sendToClient(client, ":ircserv 433 " + (client.getNickname().empty() ? "*" : client.getNickname()) +
								 " " + param + IRC::toString(IRC::ERR_NICKNAMEINUSE));
		return;
	}
	client.setNickname(param);
	if (!client.getUsername().empty() && !client.isRegistered()) {
		client.setRegistered(true);
		sendToClient(client, ":ircserv 001 " + client.getNickname() +
								 " :Welcome to the IRC server\r\n");
	}
}

void Server::handleUsername(Client& client, const std::string& param) {
	std::string username = param.substr(0, param.find(' '));
	client.setUsername(username);
	if (!client.getNickname().empty() && !client.isRegistered()) {
		client.setRegistered(true);
		sendToClient(client, ":ircserv 001 " + client.getNickname() +
								 " :Welcome to the IRC server\r\n");
	}
}


/////////////////////////////////////////////////////////////////////////////////
//----------------------UTILS---------------------------------------------------/
/////////////////////////////////////////////////////////////////////////////////

Client* Server::findClient(const std::string &nickname) {
    for (std::map<int, Client*>::iterator it = _clientMap.begin();
         it != _clientMap.end(); it++) {
        if (it->second->getNickname() == nickname)
            return it->second;
    }
    return NULL;
}

void Server::setNonBlocking(int fd) {
	LOG_D("Server: setNonblocking called");
	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("fcntl F_SETFL: " +
								 std::string(strerror(errno)));
}

void Server::sendToClient(Client& client, const std::string& msg) {
	client.appendToOutBuffer(msg);
	flushClient(client);
}

void Server::broadcastToChannel(Channel& channel, const std::string& msg, Client* exclude) {
	const std::map<Client*, bool>& members = channel.getMembers();
	for (std::map<Client*, bool>::const_iterator it = members.begin();
		 it != members.end(); ++it) {
		if (it->first != exclude)
			sendToClient(*it->first, msg);
	}
}

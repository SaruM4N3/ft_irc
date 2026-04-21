/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsonie <zsonie@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 17:51:15 by zsonie            #+#    #+#             */
/*   Updated: 2026/04/21 16:39:46 by zsonie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <unistd.h>
#include <cstring>
#include <string>
#include <fcntl.h>
#include "Debug.hpp"

// Constructor/Destructor
Server::Server(int port, const std::string& password)
	: _port(port), _password(password) {
	LOG_I("Constructor: Parameterized Server");
	LOG_W("Port: " + toString(_port));
	LOG_W("Password: " + _password);
	init();
}

Server::~Server() {
	LOG_I("Destructor: Server");
	// close every existing client
    for (std::map<int, Client*>::iterator it = _clientMap.begin();
		 it != _clientMap.end(); ++it) {
		close(it->first);
		delete it->second;
	}
    // only close when exist
	if (_serverFd != -1) close(_serverFd);
	if (_epFd != -1) close(_epFd);
}

//---------------------------Methods-------------------------------------

//---------------SERVER--------------

//start the server
void Server::init() {
	LOG_I("Server init");
	// socketcreation
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

	// epoll instantiation
	_epFd = epoll_create1(0);
	if (_epFd == -1)
		throw std::runtime_error("epoll: " + std::string(std::strerror(errno)));
	epollAdd(_serverFd, EPOLLIN);
}

//the server routine
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

//----------------CLIENTS------------------
void Server::acceptClient() {
	struct sockaddr_in clientAddr;
	socklen_t len = sizeof(clientAddr);

	int clientFd = accept(_serverFd, (struct sockaddr*)&clientAddr, &len);
	if (clientFd == -1) return;
	setNonBlocking(clientFd);
	epollAdd(clientFd, EPOLLIN);
	_clientMap[clientFd] = new Client(clientFd, clientAddr);
	LOG_D("New client connected: fd=" + toString(clientFd));
}

void Server::handleClient(int fd) {
	char buf[4096];
	int bytes = recv(fd, buf, sizeof(buf), 0);
	if (bytes <= 0) {
		removeClient(fd);
		return;
	}
	_clientMap[fd]->appendToBuffer(std::string(buf, bytes));
	LOG_D("Received data from fd=" + toString(fd));
}

void Server::removeClient(int fd) {
	epollDel(fd);
	close(fd);
	delete _clientMap[fd];
	_clientMap.erase(fd);
	LOG_D("Client disconnected: fd=" + toString(fd));
}

//--------------------EPOLL-----------------------------

// Add specific fd to epoll
void Server::epollAdd(int fd, uint32_t events) {
	struct epoll_event ev;
	ev.events = events;
	ev.data.fd = fd;
	if (epoll_ctl(_epFd, EPOLL_CTL_ADD, fd, &ev) == -1)
		throw std::runtime_error("epoll_ctl ADD: " +
								 std::string(strerror(errno)));
}

// Delete specific fd from epoll
void Server::epollDel(int fd) {
	if (epoll_ctl(_epFd, EPOLL_CTL_DEL, fd, NULL) == -1)
		throw std::runtime_error("epoll_ctl DEL: " +
								 std::string(strerror(errno)));
}

//----------------------UTILS-----------------------

// Add nonblocking flag 
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

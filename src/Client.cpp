/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaamonch <vaamonch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 14:33:19 by zsonie            #+#    #+#             */
/*   Updated: 2026/06/23 20:44:17 by vaamonch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Debug.hpp"
#include <sys/socket.h>

// constructor/destructor
Client::Client(int fd, struct sockaddr_in addr)
	: _fd(fd), _addr(addr), _authenticated(false), _registered(false) {
	(void)_addr;
	LOG_D("Client Constructor");
}
Client::~Client() { LOG_D("Client Destructor"); }

// message
std::string Client::extractMessage() {
	size_t pos = _inBuffer.find("\r\n");
	std::string msg = _inBuffer.substr(0, pos);
	_inBuffer.erase(0, pos + 2);
	LOG_D("client message: " + msg);
	return msg;
}

// get
int Client::getFd() const { return _fd; }
std::string Client::getUsername() const { return _username; }
std::string Client::getNickname() const { return _nick; }
bool Client::isMessageReceived() const {
	return (_inBuffer.find("\r\n") != std::string::npos);
}
bool Client::isAuthenticated() const { return _authenticated; }
bool Client::isRegistered() const { return _registered; }
bool Client::isWaiting() const { return !_outBuffer.empty(); }

// set
void Client::setNickname(std::string nick) { _nick = nick; }
void Client::setUsername(std::string username) { _username = username; }
void Client::setAuthenticated(bool auth) { _authenticated = auth; }
void Client::setRegistered(bool reg) { _registered = reg; }

// utils
void Client::appendToBuffer(const std::string& data) { _inBuffer += data; }
void Client::appendToOutBuffer(const std::string& data) { _outBuffer += data; }

void Client::flushOutBuffer() {
	if (_outBuffer.empty()) return;
	int bytes = send(_fd, _outBuffer.c_str(), _outBuffer.size(), 0);
	if (bytes > 0)
		_outBuffer.erase(0, bytes);
}


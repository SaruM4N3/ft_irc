/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsonie <zsonie@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 14:33:19 by zsonie            #+#    #+#             */
/*   Updated: 2026/04/21 17:27:36 by zsonie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Debug.hpp"

Client::Client(int fd, struct sockaddr_in addr) : _fd(fd), _addr(addr) {
	LOG_I("Client Constructor");
}

Client::~Client() {
	LOG_I("Client Destructor");
}

int Client::getFd() const { return _fd; }

void Client::appendToBuffer(const std::string& data) { _inBuffer += data; }

bool Client::isMessageReceived() const {
	bool ret;
	if (_inBuffer.find("\r\n") != std::string::npos)
		ret = true;
	else
		ret = false;
	return ret;
}

std::string Client::extractMessage() {
    size_t pos = _inBuffer.find("\r\n");
	std::string msg = _inBuffer;
	msg.erase(pos, 2);
	_inBuffer.erase(0, pos + 2);
	LOG_D("client message: " + msg);
	return msg;
}


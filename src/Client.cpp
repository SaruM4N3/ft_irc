/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsonie <zsonie@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 14:33:19 by zsonie            #+#    #+#             */
/*   Updated: 2026/04/20 21:31:30 by zsonie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int fd, struct sockaddr_in addr)
    : _fd(fd), _addr(addr) {}

Client::~Client() {}

int Client::getFd() const { return _fd; }

void Client::appendToBuffer(const std::string& data) {
    _inBuffer += data;
}

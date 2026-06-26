/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsonie <zsonie@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 14:33:22 by zsonie            #+#    #+#             */
/*   Updated: 2026/06/27 01:50:37 by zsonie           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <netinet/in.h>
#include <map>

class Client {
   public:
	// constructor/destructor
	Client(int fd, struct sockaddr_in addr);
	~Client();

	//
	void appendToBuffer(const std::string& data);
	void appendToOutBuffer(const std::string& data);
	std::string extractMessage();
	/**
	 *	@brief Cuts a maxLineLen-sized fragment off an oversized PRIVMSG
	 *	and re-injects it as its own complete message.
	 */
	bool trySplitOversizedMessage(size_t maxLineLen);
	bool nextLineFits(size_t maxLineLen) const;

	// get
	int getFd() const;
	std::string getNickname() const;
	std::string getUsername() const;

	// set
	void setNickname(std::string nick);
	void setUsername(std::string username);
	void setAuthenticated(bool auth);
	void setRegistered(bool reg);

	// utils
	bool isMessageReceived() const;
	bool isAuthenticated() const;
	bool isRegistered() const;
	bool isWaiting() const;
	void flushOutBuffer();
	size_t inBufferSize() const;

   private:
	int _fd;
	struct sockaddr_in _addr;
	std::string _inBuffer;
	std::string _outBuffer;

	bool _authenticated;
	bool _registered;

	std::string _username;
	std::string _nick;
};

#endif
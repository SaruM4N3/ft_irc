/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsonie <zsonie@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 14:33:22 by zsonie            #+#    #+#             */
/*   Updated: 2026/04/21 19:47:59 by zsonie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <netinet/in.h>

class Client {
   public:
	// constructor/destructor
	Client(int fd, struct sockaddr_in addr);
	~Client();

	//
	void appendToBuffer(const std::string& data);
	std::string extractMessage();

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

   private:
	int _fd;
	struct sockaddr_in _addr;
	std::string _inBuffer;

	bool _authenticated;
	bool _registered;

	std::string _username;
	std::string _nick;
};

#endif
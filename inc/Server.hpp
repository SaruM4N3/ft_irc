/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsonie <zsonie@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 17:51:20 by zsonie            #+#    #+#             */
/*   Updated: 2026/04/21 19:50:49 by zsonie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

// INC
#include "Client.hpp"
#include <map>
#include <netinet/in.h>
#include <string>
#include <sys/epoll.h>

// LIMIT
#ifndef MAX_EVENTS
#define MAX_EVENTS 64
#endif

class Server {
   public:
	Server(int port, const std::string& password);
	~Server();

	void update();

   private:
	void init();

	// EPOLL
	void epollAdd(int fd, uint32_t events);
	void epollDel(int fd);

	// CLIENT
	void acceptClient();
	void handleClient(int fd);
	void removeClient(int fd);

	void processMessage(Client& client, const std::string& message);
	void sendToClient(Client& client, const std::string& msg);

	// CMDS
	void handlePass(Client& client, const std::string& param);
	void handleUsername(Client& client, const std::string& param);
	void handleNickname(Client& client, const std::string& param);
  
	// UTILS
	void setNonBlocking(int fd);

	// VAR
	int _port;
	std::string _password;
	int _serverFd;
	int _epFd;
	std::map<int, Client*> _clientMap;
};

#endif
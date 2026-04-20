/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsonie <zsonie@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 17:51:20 by zsonie            #+#    #+#             */
/*   Updated: 2026/04/20 21:27:25 by zsonie           ###   ########.fr       */
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
  Server(int port, const std::string &password);
  ~Server();

  void update();

private:
  void init();
  void setNonBlocking(int fd);
  void epollAdd(int fd, uint32_t events);
  void epollDel(int fd);

  void acceptClient();
  void handleClient(int fd);
  void removeClient(int fd);

  void processMessage(Client &client, const std::string &message);

  int _port;
  std::string _password;
  int _serverFd;
  int _epFd;
  std::map<int, Client *> _clientMap;
};

#endif
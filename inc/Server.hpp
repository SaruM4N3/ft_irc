/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsonie <zsonie@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 17:51:20 by zsonie            #+#    #+#             */
/*   Updated: 2026/04/23 17:17:16 by zsonie           ###   ########.fr       */
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

	/**
	 *	@brief Simply the main loop of the server.
	 */
	void update();

   private:
	/**
	 *	@brief Initialisation of the server.
	 */
	void init();

	/////////////////////////////////////////////////////////////////////////////////
	//----------------------EPOLL---------------------------------------------------/
	/////////////////////////////////////////////////////////////////////////////////
	/**
	 *	@brief Ask kernel to watch this fd.
	 */
	void epollAdd(int fd, uint32_t events);
	/**
	 *	@brief Ask kernel to stop watching this fd.
	 */
	void epollDel(int fd);

	/////////////////////////////////////////////////////////////////////////////////
	//----------------------CLIENT--------------------------------------------------/
	/////////////////////////////////////////////////////////////////////////////////
	/**
	 *	@brief Accept a new client by binding a new fd to it.
	 */
	void acceptClient();
	/**
	 *	@brief Receive data from client, wait for buffer to be complete and then
	 * display the message
	 */
	void handleClient(int fd);
	/**
	 *	@brief Clean everything related to this client.
	 */
	void removeClient(int fd);

	/////////////////////////////////////////////////////////////////////////////////
	//----------------------CMDS----------------------------------------------------/
	/////////////////////////////////////////////////////////////////////////////////
	/**
	 *	@brief Here's where all the magic happens,
	 *	it does parse an IRC message to the target handler.
	 *
	 *	If you have any cmds to add that's where it goes!
	 *	@note To add a new command, just add this
	 *
	 *	``` else if (cmd == "BONJOUR") handleBonjour(client, params); ```
	 */
	void processMessage(Client& client, const std::string& message);
	/**
	 *	@brief Handle the password of the client.

	 *	This step is the first one called in processMessage function.
	 *	Client have to input the correct password or he will get kicked of the
	 server.
	 */
	void handlePass(Client& client, const std::string& param);
	/**
	 *	@brief Handle the Nickname of the client.
	 *	This step is the 2nd one called in processMessage function.
	 *	Client Should have username and nickname to get registered and be able
	 * to interact with the server.
	 */
	void handleNickname(Client& client, const std::string& param);
	/**
	 *	@brief Handle the Username of the client.
	 *	This step is the 3rd one called in processMessage function.
	 *	Client Should have username and nickname to get registered and be able
	 * to interact with the server.
	 */
	void handleUsername(Client& client, const std::string& param);

	/////////////////////////////////////////////////////////////////////////////////
	//----------------------UTILS---------------------------------------------------/
	/////////////////////////////////////////////////////////////////////////////////
	/**
	 *	@brief Just an utility function to send a message to the client.
	 */
	void sendToClient(Client& client, const std::string& msg);
	/**
	 *	@brief Add a flag O_NONBLOCK to already existing flags.
	 *	It's used to cahnge filedescriptors to avoid I/O calls to wait for data.
	 *	Instead it's going to return immediatly.
	 */
	void setNonBlocking(int fd);

	/////////////////////////////////////////////////////////////////////////////////
	//----------------------VARS----------------------------------------------------/
	/////////////////////////////////////////////////////////////////////////////////

	int _port;							///< TCP port the server's gonna listen
	std::string _password;				///< Password of the server
	int _serverFd;						///< Listening fd
	int _epFd;							///< Epoll fd
	std::map<int, Client*> _clientMap;	///< Client map
};

#endif
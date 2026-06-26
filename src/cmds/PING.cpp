/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PING.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsonie <zsonie@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 01:51:20 by zsonie            #+#    #+#             */
/*   Updated: 2026/06/27 01:51:21 by zsonie           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"

void Server::handlePing(Client &client, const std::string &param){
	std::string trailing = param.substr(param.find(':') + 1);
	sendToClient(client, ":ircserv PONG :" + trailing + "\r\n");
}
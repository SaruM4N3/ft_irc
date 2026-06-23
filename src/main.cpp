/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaamonch <vaamonch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 14:32:20 by zsonie            #+#    #+#             */
/*   Updated: 2026/06/23 21:57:59 by vaamonch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <cstdlib>
#include <iostream>

int main(int argc, char** argv) {
	if (argc != 3) {
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}
	int port = std::atoi(argv[1]);
	if (port <= 0 || port > 65535) {
		std::cerr << "Invalid port" << std::endl;
		return 1;
	}
	try {
		Server server(port, argv[2]);
		server.update();
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}

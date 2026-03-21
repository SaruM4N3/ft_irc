/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsonie <zsonie@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 14:32:20 by zsonie            #+#    #+#             */
/*   Updated: 2026/03/20 23:13:54 by zsonie           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <cstdlib>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }
    int port = std::atoi(argv[1]);
    if (port <= 0 || port > 65535)
    {
        std::cerr << "Invalid port" << std::endl;
        return 1;
    }
    
    int sockFd;
    sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFd == -1)
    {
        perror("Socket\n");
        return 1;
    }

    int opt = 1;
    setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(1234);

    if (bind(sockFd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Bind\n");
        return 1;
    }

    if (listen(sockFd, 6) == -1)
    {
        perror("Listen\n");
        return 1;
    }

    struct sockaddr_in clientAddr;
    socklen_t length = sizeof(clientAddr);
    int clientFd = accept(sockFd, (struct sockaddr *)&clientAddr, &length);
    if (clientFd == -1)
    {
        perror("Accept\n");
        return 1;
    }
    
    std::cout << "new connection" << std::endl;
    
    char buffer[4096];
    size_t bytes_read;
    
    while ((bytes_read = read(clientFd, buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, bytes_read);    
    }

    close(clientFd);
    close(sockFd);
    
    // Server server(port, argv[2]);
    // server.run();
    return 0;
}

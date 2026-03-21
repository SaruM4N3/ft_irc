/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsonie <zsonie@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 17:51:15 by zsonie            #+#    #+#             */
/*   Updated: 2026/03/20 23:05:05 by zsonie           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
{
    std::cout << CYAN << "Default constructor called on "
              << GREEN << this->_name
              << RESET << std::endl;
}

Server::Server(std::string name): _name(name)
{
    std::cout << CYAN << "Paramaterized constructor called on "
              << GREEN << this->_name
              << RESET << std::endl;
}

Server::Server(const Server &copy)
{
    (void)copy;
    std::cout << CYAN << "Copy constructor called on "
              << GREEN << this->_name
              << RESET << std::endl;
}

Server &Server::operator=(const Server &copy)
{
    if (this != &copy)
        this->_name = copy._name;
    std::cout << CYAN << "Copy assignment operator called on "
              << GREEN << this->_name
              << RESET << std::endl;
    return *this;
}

Server::~Server()
{
    std::cout << RED << "Destructor called on "
              << GREEN << this->_name
              << RESET << std::endl;
}

std::string Server::getName()
{
    return this->_name;
}


// int Server::connect()
// {
//     try{
        
//     }
//     catch(std::exception &e)
//     {
        
//     }
// }
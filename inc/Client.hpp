/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsonie <zsonie@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 14:33:22 by zsonie            #+#    #+#             */
/*   Updated: 2026/04/20 21:31:23 by zsonie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <netinet/in.h>

class Client {
public:
    Client(int fd, struct sockaddr_in addr);
    ~Client();

    void appendToBuffer(const std::string& data);
    int getFd() const;
    
private:
    int _fd;
    struct sockaddr_in _addr;
    std::string _nick;
    std::string _inBuffer;
};

#endif
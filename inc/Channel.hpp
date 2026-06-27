/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaamonch <vaamonch@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 01:50:30 by zsonie            #+#    #+#             */
/*   Updated: 2026/06/27 02:42:25 by vaamonch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "Client.hpp"
#include "Debug.hpp"
# include <string>
# include <map>
# include <vector>
# include <set>

class Channel {
    public:
        Channel(){};
        Channel(const std::string &name);
        ~Channel();

        // Membership
        void    addMember(Client *client, bool isOp);
        void    removeMember(Client *client);
        void    addInvitation(std::string nickname);
        void    removeInvitation(std::string nickname);
        /** @brief verrify that Channel has this client*/
        bool    hasMember(Client *client) const;
        bool    isOperator(Client *client) const;
        bool    isEmpty() const;
        void    OpPrivilege(std::string nickname, bool status);

        // Getters
        const std::map<Client*, bool> &getMembers()      const;
        const std::string           &getName()          const;
        const std::string           &getTopic()         const;
        const std::string           &getTopicSetter()   const;
        const std::string           &getPassword()      const;
        const time_t                &getTopicTime()     const;
        size_t                       getMemberCount()   const;

        int                          getUserLimit()     const;
        bool                         isInviteOnly()     const;
        bool                         isTopicLocked()    const;
        std::string                  getMemberList()    const;
        bool                         getBOTStatus()     const;

        // Setters (called by MODE handler) 
	    void    setTopic(const std::string &topic, const std::string &setter);
        void    setName(const std::string &name);
        void    setPassword(const std::string &password);
        void    setUserLimit(int limit);
        void    setInviteOnly(bool val);
        void    setTopicLocked(bool val);
        void    setBOTStatus(bool val);

        // Invite list (for +i mode)
        void    addInvite(const std::string &nick);
        bool    isInvited(const std::string &nick) const;

    private:
        std::string             _name;
        std::string             _topic;
        std::string             _topicSetter;
        time_t                  _topicTime;
        std::string             _password;

        std::map<Client*, bool> _members;    // client → is_operator
        std::set<std::string>   _inviteList; // nicks invited via INVITE

        bool    _inviteOnly;
        bool    _topicLocked;
        int     _userLimit;
        bool    _BOTStatus;
};

# endif
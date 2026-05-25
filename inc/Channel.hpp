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
        /** @brief verrify that Channel has this client*/
        bool    hasMember(Client *client) const;
        bool    isOperator(Client *client) const;
        bool    isEmpty() const;

        // Broadcast
        /** @brief send message to Channel */
        void    broadcast(const std::string &msg);

        // Getters
        const std::string           &getName()     const;
        const std::string           &getTopic()    const;
        const std::string           &getPassword() const;
        int                          getUserLimit() const;
        bool                         isInviteOnly() const;
        bool                         isTopicLocked() const;
        std::string                  getMemberList() const;

        // Setters (called by MODE handler) 
        void    setName(const std::string &name);
        void    setTopic(const std::string &topic);
        void    setPassword(const std::string &password);
        void    setUserLimit(int limit);
        void    setInviteOnly(bool val);
        void    setTopicLocked(bool val);

        // Invite list (for +i mode)
        void    addInvite(const std::string &nick);
        bool    isInvited(const std::string &nick) const;

    private:
        std::string             _name;
        std::string             _topic;
        std::string             _password;

        std::map<Client*, bool> _members;    // client → is_operator
        std::set<std::string>   _inviteList; // nicks invited via INVITE

        bool    _inviteOnly;
        bool    _topicLocked;
        int     _userLimit;
};

# endif
# include "Channel.hpp"

Channel::Channel(const std::string &name)
    : _name(name),
      _topic("UNDEFINED"),
      _password(""),
      _inviteOnly(false),
      _topicLocked(true),
      _userLimit(100)
{}

Channel::~Channel(){
}


// --------------------------- MEMBERSHIP ------------------------------------------

void Channel::addMember(Client *client, bool isOp) {
    _members[client] = isOp;
    if (isInvited(client->getNickname()))
        removeInvitation(client->getNickname());
}
void    Channel::removeMember(Client *client){
    _members.erase(client);
}

void Channel::addInvitation(std::string nickname) {
    if (!isInvited(nickname))
        _inviteList.insert(nickname); 
}
void Channel::removeInvitation(std::string nickname) {
    std::set<std::string>::const_iterator it = _inviteList.find(nickname);
    if (it != _inviteList.end())
        _inviteList.erase(it);
}

bool    Channel::isEmpty() const
{
    if (_members.size() == 0)
        return true;
    return false;
}


bool Channel::hasMember(Client *client) const
{
    std::map<Client*, bool>::const_iterator it = _members.find(client);
    if (it == _members.end())
        return false;
    return true;
}

bool    Channel::isOperator(Client *client) const {

    std::map<Client*, bool>::const_iterator it = _members.find(client);
    
    if (it == _members.end()) {
        return (false);
    }
    return (it->second);
}

// --------------------------- BROADCAST ------------------------------------------

void Channel::broadcast(const std::string &msg){
    for (std::map<Client*, bool>::const_iterator it = _members.begin(); it != _members.end(); it++){
        ::send(it->first->getFd(), msg.c_str(), msg.size(), 0);
    }
}

// --------------------------- GETTERS ------------------------------------------

const std::string   &Channel::getName()     const{
    return(this->_name);
}

const std::string   &Channel::getTopic()    const{
    return(this->_topic);
}

const std::string   &Channel::getPassword() const{
    return(this->_password);
}

int                  Channel::getUserLimit() const{
    return (this->_userLimit);
}

bool                 Channel::isInviteOnly() const{
    return (this->_inviteOnly);
}

bool                 Channel::isTopicLocked() const{
    return (this->_topicLocked);
}

std::string Channel::getMemberList() const {
    std::string list;
    for (std::map<Client*, bool>::const_iterator it = _members.begin(); it != _members.end(); it++) {
        if (!list.empty())
            list += " ";
        if (it->second)        // bool = true → operator
            list += "@";
        list += it->first->getNickname();
    }
    return list;
}

// --------------------------- SETTERS ------------------------------------------

void Channel::setName(const std::string &name){
    this->_name = name;
}
void    Channel::setTopic(const std::string &topic){
    this->_topic = topic;
}
void    Channel::setPassword(const std::string &password){
    this->_password = password;
}
void    Channel::setUserLimit(int limit){
    this->_userLimit = limit;
}
void    Channel::setInviteOnly(bool val){
    this->_inviteOnly = val;
}
void    Channel::setTopicLocked(bool val){
    this->_topicLocked = val;
}

// --------------------------- INVITES ------------------------------------------

void    Channel::addInvite(const std::string &nick){
    _inviteList.insert(nick);
}

bool    Channel::isInvited(const std::string &nick) const{
    std::set<std::string>::const_iterator it = _inviteList.find(nick);
    if ( it != _inviteList.end())
        return (true);
    return (false);
}
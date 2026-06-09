#include "Server.hpp"
#include "utils.hpp"

void Server::handleCom(Client &client, const std::string &param) {
    
    std::string target;
    std::string msg;
    
    size_t pos = param.find(' ');
    if ( pos == std::string::npos){
        sendToClient(client, ":ircserv 411 " + client.getNickname() + IRC::toString(IRC::ERR_NORECIPIENT));
        return ;
    }
    
    target  = param.substr(0, pos);
    msg     = param.substr(pos + 1);

    if ( target.empty() || msg.empty()){
        sendToClient(client, ":ircserv 411 " + client.getNickname() + IRC::toString(IRC::ERR_NORECIPIENT));
        return ;
    }
    
    if (target[0] == '#') {
        if (!_channelList[target].hasMember(&client)) {
            sendToClient(client, ":ircserv 442 " + client.getNickname() + " " + target + IRC::toString(IRC::ERR_NOTONCHANNEL));
            return ;
           }
        _channelList[target].broadcastE(":" + client.getNickname() + "!" + client.getUsername() + "@localhost PRIVMSG " + target + " " + msg + "\r\n", &client);
    }
    else {
        Client *dest = findClient(target);
        if (!dest) {
            sendToClient(client, ":ircserv 401 " + client.getNickname() + " " + target + IRC::toString(IRC::ERR_NOSUCHNICK));
            return ;
        }
        sendToClient(*dest, ":" + client.getNickname() + "!" + client.getUsername() + "@localhost PRIVMSG " + target + " " + msg + "\r\n");
        return;
    }
}
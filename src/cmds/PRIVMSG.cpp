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
        std::map<std::string, Channel>::iterator chanIt = _channelList.find(target);
        if (chanIt == _channelList.end()) {
            sendToClient(client, ":ircserv 403 " + client.getNickname() + " " + target + IRC::toString(IRC::ERR_NOSUCHCHANNEL));
            return ;
        }
        Channel &c = chanIt->second;
        if (!c.hasMember(&client)) {
            sendToClient(client, ":ircserv 442 " + client.getNickname() + " " + target + IRC::toString(IRC::ERR_NOTONCHANNEL));
            return ;
            }
        broadcastToChannel(c, ":" + client.getNickname() + "!" + client.getUsername() + "@localhost PRIVMSG " + target + " " + msg + "\r\n", &client);

        //*our absolute banger of a BOT
        if (msg.find("quoi") != std::string::npos){
            std::string _botmsg = msg;
            while (_botmsg.find("quoi") != std::string::npos)
                _botmsg.replace(_botmsg.find("quoi"), 4, "feur" );

            broadcastToChannel(c, ":BOT!BOT@localhost PRIVMSG " + target + " " + _botmsg + "\r\n");
        }
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
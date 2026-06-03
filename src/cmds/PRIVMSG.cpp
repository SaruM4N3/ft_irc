#include "Server.hpp"

void Server::handleCom(Client &client, const std::string &param) {
	
	std::string target;
	std::string msg;
	
	size_t pos = param.find(' ');
	if ( pos == std::string::npos){
		sendToClient(client, ":server 411 " + client.getNickname() + " :No text to send\r\n");
		return ;
	}
	
	target 	= param.substr(0, pos);
	msg 	= param.substr(pos + 1);

	if ( target.empty() || msg.empty()){
		sendToClient(client, ":server 411 " + client.getNickname() + " :No text to send\r\n");
		return ;
	}
	
	if (target[0] == '#') {
		if (!_channelList[target].hasMember(&client)) {
        	sendToClient(client, ":ircserv 442 " + client.getNickname() + " " + target + " :You're not on that channel\r\n");
        	return ;
   		}
		_channelList[target].broadcast(":" + client.getNickname() + " PRIVMSG " + target + " :" + msg + "\r\n");
	}
	else {
        Client *dest = findClient(target);
        if (!dest) {
            sendToClient(client, ":server 401 " + client.getNickname() + " " + target + " :No such nick\r\n");
            return ;
        }
        sendToClient(*dest, ":" + client.getNickname() + " PRIVMSG " + target + " :" + msg + "\r\n");
		return;
    }
}

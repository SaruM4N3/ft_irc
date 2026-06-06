# include "Server.hpp"

void Server::handlePing(Client &client, const std::string &param){
	std::string trailing = param.substr(param.find(':') + 1);
	sendToClient(client, ":ircserv PONG :" + trailing + "\r\n");
}
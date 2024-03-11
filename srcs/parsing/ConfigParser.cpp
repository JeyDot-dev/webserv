/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsohler <lsohler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 11:14:07 by lsohler@stu       #+#    #+#             */
/*   Updated: 2024/03/11 16:09:36 by lsohler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "WebServ.hpp" 

std::string location_config[] = {
	"location",
	"methods",
	"root",
	"cgi",
	"upload",
	"access"
};

std::string server_config[] = {
	"listen",
	"server_name",
	"access_log",
	"error_log",
	"error_page",
	"root",
	"index",
	"client_max_body_size",
	"location"
};



void	handleListen(WebServ &server, std::vector<std::string> tokens) {
	std::vector<std::string>::iterator	it;
	unsigned int						port;

	for (it = tokens.begin(); it != tokens.end(); it++) {
		port = atoi(it->c_str());
		if (port == 0)
			throw std::runtime_error("Invalid listen port");
		server.setPort(port);
	}
}

void	handleServerName(WebServ &server, std::vector<std::string> tokens) {

}

void	handleAccesLog(WebServ &server, std::vector<std::string> tokens) {

}

void	handleErrorLog(WebServ &server, std::vector<std::string> tokens) {

}

void	handleErrorPage(WebServ &server, std::vector<std::string> tokens) {

}

void	handleRoot(WebServ &server, std::vector<std::string> tokens) {

}

void	handleIndex(WebServ &server, std::vector<std::string> tokens) {

}

void	handleClientMaxBodySize(WebServ &server, std::vector<std::string> tokens) {

}

void	handleLocation(WebServ &server, std::stringstream block) {

}

void	serverConfigSwitcher(WebServ &server, std::vector<std::string> tokens, std::stringstream block) {
	for (int i = 0; i < server_config.size(); i++) {
		if (server_config[i] == tokens[0])
	}
	switch (i) {
		case 0:
			handleListen(server, tokens);
			break;
		case 1:
			handleServerName(server, tokens);
			break;
		case 2:
			handleAccesLog(server, tokens);
			break;
		case 3:
			handleErrorLog(server, tokens);
			break;
		case 4:
			handleErrorPage(server, tokens);
			break;
		case 5:
			handleRoot(server, tokens);
			break;
		case 6:
			handleIndex(server, tokens);
			break;
		case 7:
			handleClientMaxBodySize(server, tokens);
			break;
		case 8:
			handleLocation(server, block);
			break;
		default:
			break;
	}
}

std::stringstream	getBracketBlock(std::ifstream filestream) {
	std::stringstream	block;
	unsigned int		openBracket = 1;
	char				ch;
	while (filestream.get(ch) && openBracket) {
		block << ch;
		if (ch == '{')
			openBracket++;
		else if (ch == '}')
			openBracket--;
	}
	if (openBracket)
		throw std::runtime_error("Opened Bracket in config file");
	return	block;
}


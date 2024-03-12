/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsohler <lsohler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 11:14:07 by lsohler@stu       #+#    #+#             */
/*   Updated: 2024/03/12 13:00:59 by lsohler          ###   ########.fr       */
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
	"access",
	""
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
	"location",
	""
};

int	stringArraySize(std::string *array) {
	int i = 0;

	while (!array[i].empty()) {
		i++;
	}
	return i;
}

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
	(void)server;
	(void)tokens;
	std::cout << "handleServerName" << std::endl;
}

void	handleAccesLog(WebServ &server, std::vector<std::string> tokens) {
	(void)server;
	(void)tokens;
	std::cout << "handleAccesLog" << std::endl;
}

void	handleErrorLog(WebServ &server, std::vector<std::string> tokens) {
	(void)server;
	(void)tokens;
	std::cout << "handleErrorLog" << std::endl;
}

void	handleErrorPage(WebServ &server, std::vector<std::string> tokens) {
	(void)server;
	(void)tokens;
	std::cout << "handleErrorPage" << std::endl;
}

void	handleRoot(WebServ &server, std::vector<std::string> tokens) {
	(void)server;
	(void)tokens;
	std::cout << "handleRoot" << std::endl;
}

void	handleIndex(WebServ &server, std::vector<std::string> tokens) {
	(void)server;
	(void)tokens;
	std::cout << "handleIndex" << std::endl;
}

void	handleClientMaxBodySize(WebServ &server, std::vector<std::string> tokens) {
	(void)server;
	(void)tokens;
	std::cout << "handleClientMaxBodySize" << std::endl;
}

void	handleLocation(WebServ &server, std::stringstream block) {
	(void)server;
	(void)tokens;
	std::cout << "handleLocation" << std::endl;
}

void	serverConfigSwitcher(int i, std::vector<std::string> tokens) {
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
			handleLocation(server, tokens);
			break;
		default:
			break;
	}
}

void	serverConfigParser(WebServ &server, std::vector<std::vector<std::string>> tokensVector) {
	for (std::vector<std::vector>::iterator vectorIt = tokensVector.begin(); vectorIt != tokensVector.end(); vectorIt++) {
		for (int i = 0; i < stringArraySize(server_config); i++) {
			if (*vectorIt[0] == server_config[i]) {
				serverConfigSwitcher(i, *vectorIt);
			}
		}
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

std::vector<std::vector<std::string>>	serverConfigTokenizer(std::ifstream &filestream) {
	std::vector<std::vector<std::string>>	serverTokensVector;
	std::vector<std::string>				serverTokens;
	std::string								line;

	while (std::getline(filestream, line)) {
		std::string			token;
		std::istringstream	iss(line);
		if (line.find("{")) {
			bool	openBracket = true;
			while (openBracket) {
				
			}
		}
		else {
			while (iss >> token) {
				serverTokens.push_back(token);
			}
		}
		serverTokensVector.push_back(serverTokens);
		serverTokens.clear();
	}
}

std::map<int, WebServ>	configFileParser(std::string filename) {

}
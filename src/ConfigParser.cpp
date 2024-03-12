/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsohler <lsohler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 11:14:07 by lsohler@stu       #+#    #+#             */
/*   Updated: 2024/03/12 15:48:39 by lsohler          ###   ########.fr       */
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
	(void)server;
	(void)tokens;
	std::cout << "handleListen" << std::endl;
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

void	handleLocation(WebServ &server, std::vector<std::string> tokens) {
	(void)server;
	(void)tokens;
	std::cout << "handleLocation" << std::endl;
}

void	serverConfigSwitcher(int i, std::vector<std::string> tokens, WebServ &server) {
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

std::vector<std::string>	configFileTokenizer(std::string filename) {
	std::vector<std::string> tokens;

	std::ifstream filestream(filename.c_str());
	if (!filestream.is_open()) {
		std::cerr << "Unable to open file: " << filename << std::endl;
		return tokens;
	}

	std::string line;
	while (std::getline(filestream, line)) {
		std::istringstream iss(line);
		std::string word;
		while (iss >> word) {
			if (!word.empty() && word[0] == '#')
				break ;
			else if (!word.empty() && word.back() == ';') {
				word.pop_back();
				tokens.push_back(word);
				tokens.push_back(";");
			}
			else {
				tokens.push_back(word);
			}
		}
	}
	return tokens;
}

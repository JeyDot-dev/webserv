/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsohler@student.42.fr <lsohler>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 16:26:12 by lsohler           #+#    #+#             */
/*   Updated: 2024/03/13 19:48:44 by lsohler@stu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include "ServerConfig.hpp"

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

void	printTokenUntilSemicolon(std::vector<std::string> &tokens) {
	while (!tokens.empty()) {
		std::cout << " " << *tokens.begin();
		tokens.erase(tokens.begin());
		if (*tokens.begin() == ";") {
			tokens.erase(tokens.begin());
			break ;
		}
	}
	std::cout << std::endl;
}

void	handleListen(ServerConfig &config, std::vector<std::string> &tokens) {
	(void)config;
	std::cout << "handleListen: ";
	printTokenUntilSemicolon(tokens);
}

void	handleServerName(ServerConfig &config, std::vector<std::string> &tokens) {
	(void)config;
	std::cout << "handleServerName";
	printTokenUntilSemicolon(tokens);
}

void	handleAccessLog(ServerConfig &config, std::vector<std::string> &tokens) {
	(void)config;
	std::cout << "handleAccessLog";
	printTokenUntilSemicolon(tokens);

}

void	handleErrorLog(ServerConfig &config, std::vector<std::string> &tokens) {
	(void)config;
	std::cout << "handleErrorLog";
	printTokenUntilSemicolon(tokens);

}

void	handleErrorPage(ServerConfig &config, std::vector<std::string> &tokens) {
	(void)config;
	std::cout << "handleErrorPage";
	printTokenUntilSemicolon(tokens);

}

void	handleRoot(ServerConfig &config, std::vector<std::string> &tokens) {
	(void)config;
	std::cout << "handleRoot";
	printTokenUntilSemicolon(tokens);

}

void	handleIndex(ServerConfig &config, std::vector<std::string> &tokens) {
	(void)config;
	std::cout << "handleIndex";
	printTokenUntilSemicolon(tokens);

}

void	handleClientMaxBodySize(ServerConfig &config, std::vector<std::string> &tokens) {
	(void)config;
	std::cout << "handleClientMaxBodySize";
	printTokenUntilSemicolon(tokens);

}

void	handleLocation(ServerConfig &config, std::vector<std::string> &tokens) {
	(void)config;
	std::cout << "handleLocation";
	while (!tokens.empty() && *tokens.begin() != "}") {
		std::cout << " " << *tokens.begin();
		tokens.erase(tokens.begin());
	}
	if (*tokens.begin() == "}") {
		tokens.erase(tokens.begin());
	}
	std::cout << std::endl;
}


void handleNotRecognized(ServerConfig &config, std::vector<std::string>& tokens) {
	(void)config;
	std::cout << "handleLocation";
	printTokenUntilSemicolon(tokens);
}

typedef void (*caseHandler)(ServerConfig&, std::vector<std::string>&);

std::map<std::string, caseHandler> caseMap() {
	std::map<std::string, caseHandler> myMap;
	
	myMap["listen"] = &handleListen;
	myMap["server_name"] = &handleServerName;
	myMap["access_log"] = &handleAccessLog;
	myMap["error_log"] = &handleErrorLog;
	myMap["error_page"] = &handleErrorPage;
	myMap["root"] = &handleRoot;
	myMap["index"] = &handleIndex;
	myMap["client_max_body_size"] = &handleClientMaxBodySize;
	myMap["location"] = &handleLocation;
	myMap["NOT_RECOGNIZED"] = &handleNotRecognized;

	return myMap;	
}	

bool keyExists(const std::string& key, std::map<std::string, caseHandler> map) {
	return map.find(key) != map.end();
}

ServerConfig::ServerConfig(std::vector<std::string> tokens) {
	std::map<std::string, caseHandler> map = caseMap();
	// for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
	while (!tokens.empty()) {
		if (keyExists(*tokens.begin(), map)) {
			caseHandler function = map[*tokens.begin()];
			function(*this, tokens);
			// std::cout << "After caseHandler: " << *tokens.begin() << std::endl;
		}
		else {
			caseHandler function = map["NOT_RECOGNIZED"];
			function(*this, tokens);
		}
	}
}


ServerConfig::ServerConfig(void) : 
	_port(),
	_host(""),
	_client_max_body_size(0),
	_server_name(""),
	_access_log(""),
	_error_log(""),
	_root(""),
	_index(),
	_routes() {

}

ServerConfig::ServerConfig(ServerConfig const &other) : 
	_port(other._port),
	_host(other._host),
	_client_max_body_size(other._client_max_body_size),
	_server_name(other._server_name),
	_access_log(other._access_log),
	_error_log(other._error_log),
	_root(other._root),
	_index(other._index),
	_routes(other._routes) {

}


ServerConfig::~ServerConfig(void) {

}

ServerConfig	&ServerConfig::operator=(ServerConfig const &other) {
	if (this != &other) {
		_port = other._port;
		_host = other._host;
		_client_max_body_size = other._client_max_body_size;
		_server_name = other._server_name;
		_access_log = other._access_log;
		_error_log = other._error_log;
		_root = other._root;
		_index = other._index;
		_routes = other._routes;
	}
	return *this;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsohler <lsohler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 16:26:12 by lsohler           #+#    #+#             */
/*   Updated: 2024/03/15 17:29:54 by lsohler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include "ServerConfig.hpp"


unsigned int	maxBodySizeConverter(const std::string& maxSizeString) {
	unsigned int maxSize = std::atoi(maxSizeString.c_str());

	if (maxSize == 0)
		return DEF_MAX_BODY_SIZE;
	if (maxSizeString.size() > 2) {
		std::string suffix = maxSizeString.substr(maxSizeString.size() - 2);
		if (suffix == "ko" || suffix == "Ko") {
			maxSize *= 1024;
		} else if (suffix == "mo" || suffix == "Mo") {
			maxSize *= 1024 * 1024;
		} else if (suffix == "go" || suffix == "Go") {
			maxSize *= 1024 * 1024 * 1024;
		}
	}
	return maxSize;
}

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

typedef void (ServerConfig::*SetterFunction)(const std::string&);

void tokenSetter(std::vector<std::string>& tokens, ServerConfig &config, SetterFunction setter) {
	tokens.erase(tokens.begin());
	while (!tokens.empty()) {
		if (*tokens.begin() == ";") {
			tokens.erase(tokens.begin());
			break;
		}
		(config.*setter)(*tokens.begin());
		tokens.erase(tokens.begin());
	}
}

void	handleListen(ServerConfig &config, std::vector<std::string> &tokens) {
	tokenSetter(tokens, config, &ServerConfig::setPort);
	// (void)config;
	// std::cout << "handleListen: ";
	// printTokenUntilSemicolon(tokens);
}

void	handleClientMaxBodySize(ServerConfig &config, std::vector<std::string> &tokens) {
	tokenSetter(tokens, config, &ServerConfig::setClientMaxBodySize);
	// (void)config;
	std::cout << "handleClientMaxBodySize";
	// printTokenUntilSemicolon(tokens);
}

void	handleServerName(ServerConfig &config, std::vector<std::string> &tokens) {
	tokenSetter(tokens, config, &ServerConfig::setServerName);
	// (void)config;
	// std::cout << "handleServerName";
	// printTokenUntilSemicolon(tokens);
}

void	handleAccessLog(ServerConfig &config, std::vector<std::string> &tokens) {
	tokenSetter(tokens, config, &ServerConfig::setAccessLog);
	// (void)config;
	// std::cout << "handleAccessLog";
	// printTokenUntilSemicolon(tokens);
}

void	handleErrorLog(ServerConfig &config, std::vector<std::string> &tokens) {
	tokenSetter(tokens, config, &ServerConfig::setErrorLog);
	// (void)config;
	// std::cout << "handleErrorLog";
	// printTokenUntilSemicolon(tokens);
}

void	handleErrorPage(ServerConfig &config, std::vector<std::string> &tokens) {
	tokenSetter(tokens, config, &ServerConfig::setErrorPage);
	// (void)config;
	// std::cout << "handleErrorPage";
	// printTokenUntilSemicolon(tokens);
}

void	handleRoot(ServerConfig &config, std::vector<std::string> &tokens) {
	tokenSetter(tokens, config, &ServerConfig::setRoot);
	// (void)config;
	// std::cout << "handleRoot";
	// printTokenUntilSemicolon(tokens);
}

void	handleIndex(ServerConfig &config, std::vector<std::string> &tokens) {
	tokenSetter(tokens, config, &ServerConfig::setServerName);	
	// (void)config;
	// std::cout << "handleIndex";
	// printTokenUntilSemicolon(tokens);
}

void	tokenNotRecognized(std::vector<std::string> &tokens) {
	std::cerr << "TOKEN NOT RECOGNIZED: ";
	while (!tokens.empty()) {
		std::cerr << " " << *tokens.begin();
		tokens.erase(tokens.begin());
		if (*tokens.begin() == ";") {
			tokens.erase(tokens.begin());
			break ;
		}
	}
	std::cerr << std::endl;
}

/*------Handle Location------------*/
void	locationHandleMethods(Route &route, std::vector<std::string> &tokens) {
	tokens.erase(tokens.begin());
	while (!tokens.empty()) {
		if (*tokens.begin() == ";") {
			tokens.erase(tokens.begin());
			break;
		}
		route.methods.push_back(*tokens.begin());
		tokens.erase(tokens.begin());
	}
}

void	locationHandleRoot(Route &route, std::vector<std::string> &tokens) {
	tokens.erase(tokens.begin());
	while (!tokens.empty()) {
		if (*tokens.begin() == ";") {
			tokens.erase(tokens.begin());
			break;
		}
		route.root = *tokens.begin();
		tokens.erase(tokens.begin());
	}
}

void	locationHandleCgi(Route &route, std::vector<std::string> &tokens) {
	tokens.erase(tokens.begin());
	while (!tokens.empty()) {
		if (*tokens.begin() == ";") {
			tokens.erase(tokens.begin());
			break;
		}
		route.cgi = *tokens.begin();
		tokens.erase(tokens.begin());
	}
}

void	locationHandleUpload(Route &route, std::vector<std::string> &tokens) {
	tokens.erase(tokens.begin());
	while (!tokens.empty()) {
		if (*tokens.begin() == ";") {
			tokens.erase(tokens.begin());
			break;
		}
		route.upload = *tokens.begin();
		tokens.erase(tokens.begin());
	}
}

void	locationHandleAccess(Route &route, std::vector<std::string> &tokens) {
	tokens.erase(tokens.begin());
	while (!tokens.empty()) {
		if (*tokens.begin() == ";") {
			tokens.erase(tokens.begin());
			break;
		}
		if (*tokens.begin() == "true" || *tokens.begin() == "yes") {
			route.access = true;
		}
		else {
			route.access = false;
		}
		tokens.erase(tokens.begin());
	}
}

typedef void (*locationHandler)(Route&, std::vector<std::string>&);

std::map<std::string, locationHandler> locationMap() {
	std::map<std::string, locationHandler> myMap;
	
	myMap["methods"] = &locationHandleMethods;
	myMap["root"] = &locationHandleRoot;
	myMap["cgi"] = &locationHandleCgi;
	myMap["upload"] = &locationHandleUpload;
	myMap["access"] = &locationHandleAccess;

	return myMap;	
}

void	handleLocation(ServerConfig &config, std::vector<std::string> &tokens) {
	Route	new_route;
	std::map<std::string, locationHandler> map = locationMap();

	new_route.root = "";
	new_route.cgi = "";
	new_route.upload = "";
	new_route.access = true;
	tokens.erase(tokens.begin());
	if (*tokens.begin() != "{") {
		new_route.location = *tokens.begin();
		tokens.erase(tokens.begin());
	}
	else {
		std::cerr << "Location parsing error." << std::endl;
		exit(1);
	}
	while (!tokens.empty()) {
		if (map.find(*tokens.begin()) != map.end()) {
			locationHandler function = map[*tokens.begin()];
			function(new_route, tokens);
		}
		else if (*tokens.begin() == "}") {
			tokens.erase(tokens.begin());
			break ;
		}
		else {
			tokenNotRecognized(tokens);
		}
	}
	config.setRoutes(new_route);
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

	return myMap;	
}	

/*-----------CONSTRUCTOR DESTRUCTOR------------*/
bool keyExists(const std::string& key, std::map<std::string, caseHandler> map) {
	return map.find(key) != map.end();
}

ServerConfig::ServerConfig(std::vector<std::string> tokens) : 
	_port(),
	_client_max_body_size(DEF_MAX_BODY_SIZE),
	_server_name(""),
	_access_log(""),
	_error_log(""),
	_root(""),
	_index(),
	_routes() {
	std::map<std::string, caseHandler> map = caseMap();
	// for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
	while (!tokens.empty()) {
		if (keyExists(*tokens.begin(), map)) {
			caseHandler function = map[*tokens.begin()];
			function(*this, tokens);
			// std::cout << "After caseHandler: " << *tokens.begin() << std::endl;
		}
		else {
			tokenNotRecognized(tokens);
		}
	}
}

ServerConfig::ServerConfig(void) : 
	_port(),
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

/*--------------TESTING FUNCTIONS--------------*/

void	ServerConfig::printServerConfig(void) {
	std::cout << "Port:";
	for (size_t i = 0; i < _port.size(); ++i) {
		std::cout << " " << _port[i];
	}
	std::cout << std::endl;
	std::cout << "Client Max Body Size: " << _client_max_body_size << std::endl;
	std::cout << "Server Name: " << _server_name << std::endl;
	std::cout << "Access Log: " << _access_log << std::endl;
	std::cout << "Error Log: " << _error_log << std::endl;
	std::cout << "Root: " << _root << std::endl;
		std::cout << "Index:";
	for (size_t i = 0; i < _index.size(); ++i) {
		std::cout << " " << _index[i];
	}
	std::cout << std::endl;
	std::cout << "Routes:" << std::endl;
	for (std::map<std::string, Route>::const_iterator it = _routes.begin(); it != _routes.end(); ++it) {
		const Route& route = it->second;
		std::cout << "  Location: " << it->first << std::endl;
		std::cout << "	Methods:";
		for (size_t i = 0; i < route.methods.size(); ++i) {
			std::cout << " " << route.methods[i];
		}
		std::cout << std::endl;
		std::cout << "	Root: " << route.root << std::endl;
		std::cout << "	CGI: " << route.cgi << std::endl;
		std::cout << "	Upload: " << route.upload << std::endl;
		std::cout << "	Access: " << (route.access ? "true" : "false") << std::endl;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsohler <lsohler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 16:26:12 by lsohler           #+#    #+#             */
/*   Updated: 2024/03/12 19:49:18 by lsohler          ###   ########.fr       */
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

// static int	stringArraySize(std::string *array) {
// 	int i = 0;

// 	while (!array[i].empty()) {
// 		i++;
// 	}
// 	return i;
// }

// ServerConfig::ServerConfig(std::vector<std::string> tokens) {
// 	bool bracketBlock = true;

// 	for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); it++) {
// 		std::cout << "Token: " << *token << std::endl;
// 		for (int i = 0; i < stringArraySize(server_config); i++) {
// 			if (*token == server_config[i])
// 				serverConfigSwitcher(i, token, *this);
// 			else
// 				tokenNotRecognized(token);
// 		}
// 	}
// }


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

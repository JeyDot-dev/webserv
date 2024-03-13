/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsohler@student.42.fr <lsohler>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 16:25:31 by lsohler           #+#    #+#             */
/*   Updated: 2024/03/13 17:23:51 by lsohler@stu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/*
**==========================
**		Include
**==========================
*/
#include <iostream>
#include <string>
#include <vector>
#include <map>


struct Route {
	std::string					location;
	std::vector<std::string>	methods;
	std::string					root;
	std::string					cgi;
	std::string					upload;
	bool						access;
};

class ServerConfig {

	private:
		std::vector<unsigned int>		_port;
		std::string						_host;
		unsigned int					_client_max_body_size;
		std::string						_server_name;
		std::string						_access_log;
		std::string						_error_log;
		std::string						_root;
		std::vector<std::string>		_index;
		std::map<std::string, Route>	_routes;
		
	public:
		ServerConfig(void);
		ServerConfig(ServerConfig const &other);
		ServerConfig(std::vector<std::string> tokens);
		~ServerConfig(void);
		ServerConfig	&operator=(ServerConfig const &other);

		void	setPort(const unsigned int port) { _port.push_back(port); }
		void	setClientMaxBodySize(const unsigned int size) { _client_max_body_size = size; }
		void	setServerName(const std::string &name) { _server_name = name; }
		void	setAccessLog(const std::string &log) { _access_log = log; }
		void	setErrorLog(const std::string &log) { _error_log = log; }
		void	setRoot(const std::string &root) { _root = root; }
		void	setIndex(const std::vector<std::string> &index) { _index = index; }
		void	setRoutes(const Route &route) { _routes.insert(std::make_pair(route.location, route)); }

		void	printServerConfig(void);
		void	printServerConfigRoutes(void);

};

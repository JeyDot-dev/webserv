/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsohler <lsohler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 16:25:31 by lsohler           #+#    #+#             */
/*   Updated: 2024/03/15 14:20:37 by lsohler          ###   ########.fr       */
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

#define DEF_MAX_BODY_SIZE 10 * 1024

unsigned int	maxBodySizeConverter(const std::string &size);

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
		unsigned int					_client_max_body_size;
		std::string						_server_name;
		std::string						_access_log;
		std::string						_error_log;
		std::map<int, std::string>		_error_page;
		std::string						_root;
		std::vector<std::string>		_index;
		std::map<std::string, Route>	_routes;
		
	public:
		ServerConfig(void);
		ServerConfig(ServerConfig const &other);
		ServerConfig(std::vector<std::string> tokens);
		~ServerConfig(void);
		ServerConfig	&operator=(ServerConfig const &other);

		/*-----Set-----*/
		void	setPort(const std::string &port) { _port.push_back(atoi(port.c_str())); }
		void	setClientMaxBodySize(const std::string &size) { _client_max_body_size = maxBodySizeConverter(size); }
		void	setServerName(const std::string &name) { _server_name = name; }
		void	setAccessLog(const std::string &log) { _access_log = log; }
		void	setErrorLog(const std::string &log) { _error_log = log; }
		void	setErrorPage(const std::string &error) { _error_page.insert(std::make_pair(atoi(error.c_str()), "/" + error + ".html")); }
		void	setRoot(const std::string &root) { _root = root; }
		void	setIndex(const std::string &index) { _index.push_back(index); }
		void	setRoutes(const Route &route) { _routes.insert(std::make_pair(route.location, route)); }

		/*-----Set Utils-----*/

		void	printServerConfig(void);
		void	printServerConfigRoutes(void);

};

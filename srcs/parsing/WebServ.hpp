/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsohler@student.42.fr <lsohler>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 11:20:44 by lsohler@stu       #+#    #+#             */
/*   Updated: 2024/03/10 21:50:07 by lsohler@stu      ###   ########.fr       */
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
	std::string location;
	std::vector<std::string> methods;
	std::string root;
	std::string cgi;
	std::string upload;
};

class WebServ {

	private:
		std::vector<unsigned int>	_port;
		unsigned int				_client_max_body_size;
		std::string					_server_name;
		std::string					_access_log;
		std::string					_error_log;
		std::string					_root;
		std::vector<std::string>	_index;
		std::vector<Route>			_routes;

	public:
		WebServ(void);
		WebServ(WebServ const &other);
		~WebServ(void);
		WebServ	&operator=(WebServ const &other);

		void	setPort(unsigned int port) { _port.push_back(port); }
		void	setClientMaxBodySize(unsigned int size) { _client_max_body_size = size; }
		void	setServerName(const std::string& name) { _server_name = name; }
		void	setAccessLog(const std::string& log) { _access_log = log; }
		void	setErrorLog(const std::string& log) { _error_log = log; }
		void	setRoot(const std::string& root) { _root = root; }
		void	setIndex(const std::vector<std::string>& index) { _index = index; }
		void	setRoutes(const std::vector<Route>& routes) { _routes = routes; }

		void	printWebServ(void);
		void	printWebServRoutes(void);

};
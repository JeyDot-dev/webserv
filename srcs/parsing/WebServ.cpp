/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsohler <lsohler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 11:20:53 by lsohler@stu       #+#    #+#             */
/*   Updated: 2024/03/11 15:58:06 by lsohler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "WebServ.hpp"

WebServ::WebServ(void) : 
	_port(), 
	_client_max_body_size(0),
	_server_name(""),
	_access_log(""),
	_error_log(""),
	_root(""),
	_index(),
	_routes() {

}

WebServ::WebServ(WebServ const &other) : 
	_port(other._port),
	_client_max_body_size(other._client_max_body_size),
	_server_name(other._server_name),
	_access_log(other._access_log),
	_error_log(other._error_log),
	_root(other._root),
	_index(other._index),
	_routes(other._routes) {

}

WebServ::~WebServ(void) {

}

WebServ	&WebServ::operator=(WebServ const &other) {
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

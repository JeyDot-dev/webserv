/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsohler <lsohler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 11:20:53 by lsohler@stu       #+#    #+#             */
/*   Updated: 2024/03/12 18:49:49 by lsohler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "WebServ.hpp"
#include "ServerConfig.hpp"

WebServ::WebServ(void)  {

}

WebServ::WebServ(WebServ const &other) : _config(other._config) {

}

WebServ::WebServ(ServerConfig &new_config) {
	_config = new_config;
}

WebServ::~WebServ(void) {

}

WebServ	&WebServ::operator=(WebServ const &other) {
	if (this != &other) {
		_config = other._config;
	}
	return *this;
}

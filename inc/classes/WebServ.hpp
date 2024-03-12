/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsohler <lsohler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 11:20:44 by lsohler@stu       #+#    #+#             */
/*   Updated: 2024/03/12 16:58:20 by lsohler          ###   ########.fr       */
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
#include "ServerConfig.hpp"

class WebServ {

	private:
		ServerConfig	_config;

	public:
		WebServ(void);
		WebServ(WebServ const &other);
		WebServ(ServerConfig &new_config);
		~WebServ(void);
		WebServ	&operator=(WebServ const &other);


};

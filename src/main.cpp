/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsohler <lsohler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 15:54:52 by lsohler           #+#    #+#             */
/*   Updated: 2024/03/12 19:49:57 by lsohler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#include "ServerConfig.hpp"

std::vector<std::string>	configFileTokenizer(std::string filename);

int main(int ac, char **av) {
	if (ac != 2)
		std::cerr << "Usage: ./WebServ <config_file>" << std::endl;
	std::vector<std::string>	tokens = configFileTokenizer(av[1]);
	// std::vector<std::string>::iterator token = tokens.begin();
	// ServerConfig new_config(tokens.begin(), tokens.end());
	for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); it++) {
		std::cout << *it << " " << std::endl;
		// if (*it == "{" || *it == "}" || *it == ";")
		// 	std::cout << std::endl;
	}
}

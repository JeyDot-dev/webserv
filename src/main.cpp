/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsohler@student.42.fr <lsohler>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 15:54:52 by lsohler           #+#    #+#             */
/*   Updated: 2024/03/13 16:36:16 by lsohler@stu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#include "ServerConfig.hpp"

// std::vector<std::string>	configFileTokenizer(std::string filename);
std::map<std::string, WebServ>	configFileParser(std::string filename);


int main(int ac, char **av) {
	if (ac != 2)
		std::cerr << "Usage: ./WebServ <config_file>" << std::endl;

	configFileParser(av[1]);
	// std::vector<std::string>	tokens = configFileTokenizer(av[1]);
	// // std::vector<std::string>::iterator token = tokens.begin();
	// // ServerConfig new_config(tokens.begin(), tokens.end());
	// for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); it++) {
	// 	std::cout << *it << " " << std::endl;
	// 	// if (*it == "{" || *it == "}" || *it == ";")
	// 	// 	std::cout << std::endl;
	// }
}

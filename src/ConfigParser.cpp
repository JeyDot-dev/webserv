/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsohler <lsohler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 11:14:07 by lsohler@stu       #+#    #+#             */
/*   Updated: 2024/03/12 20:42:16 by lsohler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include "WebServ.hpp" 

std::vector<std::string>	configFileTokenizer(std::string filename) {
	std::vector<std::string> tokens;

	std::ifstream filestream(filename.c_str());
	if (!filestream.is_open()) {
		std::cerr << "Unable to open file: " << filename << std::endl;
		return tokens;
	}

	std::string line;
	while (std::getline(filestream, line)) {
		std::istringstream iss(line);
		std::string word;
		while (iss >> word) {
			if (!word.empty() && word[0] == '#')
				break ;
			else if (!word.empty() && word.back() == ';') {
				word.pop_back();
				tokens.push_back(word);
				tokens.push_back(";");
			}
			else {
				tokens.push_back(word);
			}
		}
	}
	return tokens;
}
std::vector<std::vector<std::string> >	getTokensBlock(std::vector<std::string> tokens) {
	std::vector<std::vector<std::string> > tokens_block;
	
}

std::map<std::string, WebServ>	configFileParser(std::string filename) {
	std::vector<std::vector<std::string> >	tokens_block = getTokensBlock(configFileTokenizer(filename));
	std::map<std::string, WebServ>	servers;


}

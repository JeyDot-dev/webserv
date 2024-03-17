/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsohler <lsohler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 11:14:07 by lsohler@stu       #+#    #+#             */
/*   Updated: 2024/03/15 17:45:07 by lsohler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include "Webserv.hpp"

void	printTokens(std::vector<std::string> tokens) {
	std::cout << "Tokens: ";
	for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); it++) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;
}

int	checkTokens(std::vector<std::string> tokens) {
	int	bracket_counter = 0;
	for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); it++) {
		if (*it == "{")
			bracket_counter++;
		else if (*it == "}")
			bracket_counter--;
	}
	if (!bracket_counter) {
		return 0;
	}
	std::cerr << "Parsing error: opened bracket" << std::endl;
	return 1;
}

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
			else if (!word.empty() && word.length() > 1 && (word[word.length() - 1]) == ';') {
				word.erase(word.length() - 1, 1);
				tokens.push_back(word);
				tokens.push_back(";");
			}
			else {
				tokens.push_back(word);
			}
		}
	}
	if (checkTokens(tokens)) {
		tokens.clear();
		return tokens;
	}
	return tokens;
}

std::vector<std::vector<std::string> >	getTokensBlock(std::vector<std::string> tokens) {
	std::vector<std::vector<std::string> > tokens_block;
	std::vector<std::string>::iterator it = tokens.begin();

	while (it != tokens.end()) {
		// std::cout << "========Tok " << *it << std::endl;
		if (*it == "server" && *(++it) == "{" && ++it != tokens.end()) {
			std::vector<std::string>	vector;
			int	bracket_counter = 1;
			while (it != tokens.end() && bracket_counter) {
				// std::cout << "Tok2: " << *it << std::endl;
				if (*it == "{")
					bracket_counter++;
				else if (*it == "}")
					bracket_counter--;
				if (bracket_counter)
					vector.push_back(*it);
				it++;
			}
			// std::cout << "End of Tok2" << std::endl;
			if (!vector.empty()) {
				// printTokens(vector);
				tokens_block.push_back(vector);
			}
			if (it == tokens.end())
				return tokens_block;
		}
	}
	return tokens_block;
}

std::map<std::string, Webserv>	configFileParser(std::string filename) {
	std::vector<std::vector<std::string> >	tokens_block = getTokensBlock(configFileTokenizer(filename));
	std::map<std::string, Webserv>	servers;

	for (std::vector<std::vector<std::string> >::iterator it = tokens_block.begin(); it != tokens_block.end(); it++) {
		std::cout << "Tokens server: ";
		printTokens(*it);
		std::cout << "------------New Server------------" << std::endl;
		ServerConfig	new_config(*it);
		new_config.printServerConfig();
	}
	return servers;
}

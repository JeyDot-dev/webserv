/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsohler@student.42.fr <lsohler>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 11:14:07 by lsohler@stu       #+#    #+#             */
/*   Updated: 2024/03/10 22:08:08 by lsohler@stu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

static int	currentLine = 0;

std::stringstream	getBracketBlock(std::ifstream filestream) {
	std::stringstream	block;
	unsigned int		openBracket = 1;
	char				ch;
	while (filestream.get(ch) && openBracket) {
		block << ch;
	}
	return	block;
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsohler <lsohler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 15:54:52 by lsohler           #+#    #+#             */
/*   Updated: 2024/03/11 15:59:03 by lsohler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

int main(int ac, char **av) {
	if (ac != 2)
		std::cerr << "Usage: ./WebServ <config_file>" << std::endl;
	(void)av;
}

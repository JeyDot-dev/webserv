/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsohler@student.42.fr <lsohler>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 11:14:08 by lsohler@stu       #+#    #+#             */
/*   Updated: 2024/03/10 11:15:35 by lsohler@stu      ###   ########.fr       */
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

class Animal {

	protected:
		std::string	type;

	public:
		Animal();
		Animal(Animal const &other);
		virtual ~Animal();
		Animal &operator=(Animal const &other);

		std::string	getType(void) const;
		virtual void makeSound(void) const = 0;

};
#include "Cgi.hpp"

//--------------Functions----------------//
//--------------Operators----------------//
Cgi&	Cgi::operator=(Cgi const&  rhs)
{
	if (this != &rhs)
	{
		//copy
	}
	std::cout << "Assignation operator called" << std::endl;
	return *this;
}
//--------------Constructors------------//
Cgi::Cgi(Cgi const &src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
	return ;
}

Cgi::Cgi(void)
{
	std::cout << "Default constructor called" << std::endl;
	return ;
}

Cgi::~Cgi(void)
{
	std::cout << "Destructor called" << std::endl;
	return ;
}
//--------------Non-Member--------------//

#include "Placeholder.hpp"

//--------------Functions----------------//
//--------------Operators----------------//
Placeholder&	Placeholder::operator=(Placeholder const&  rhs)
{
	if (this != &rhs)
	{
		//copy
	}
	std::cout << "Assignation operator called" << std::endl;
	return *this;
}
//--------------Constructors------------//
Placeholder::Placeholder(Placeholder const &src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
	return ;
}

Placeholder::Placeholder(void)
{
	std::cout << "Default constructor called" << std::endl;
	return ;
}

Placeholder::~Placeholder(void)
{
	std::cout << "Destructor called" << std::endl;
	return ;
}
//--------------Non-Member--------------//

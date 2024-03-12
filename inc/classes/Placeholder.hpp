#ifndef		PLACEHOLDER_HPP
# define	PLACEHOLDER_HPP
#include <iostream>

class	Placeholder
{

public:
	Placeholder(Placeholder const & src);
	Placeholder&	operator=(Placeholder const& rhs);

	Placeholder(void);
	~Placeholder(void);

private:

};

#endif

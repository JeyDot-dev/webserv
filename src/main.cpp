#include <iostream>
int main(int ac, char** av)
{
    for (int i = 1; i < ac; i++)
        std::cout<< av[i] << std::endl;
}

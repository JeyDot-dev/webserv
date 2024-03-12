#include <iostream>
#include <Socket.hpp>
#include <cstdio>
#include <cstdlib>
int main(void)
{
    Socket  a;
    Socket  b("127.0.0.1", 8080);

    if (listen(b.getFd(), 1) < 0)
    {
        perror("cannot listen");
        exit(1);
    }
    Socket::Accept(b, a);
    std::cout << "SUCCESFULLY CONNECTED" << std::endl;
}

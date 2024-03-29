#pragma once
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Webserv.hpp"
#include "ServerConfig.hpp"

std::map<int, Webserv> initMapServ(std::string file_path);
void webservInit(std::string file_path, std::map<int, Webserv> &map_serv);
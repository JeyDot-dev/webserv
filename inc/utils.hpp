#pragma once
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <dirent.h>

#include "Webserv.hpp"
#include "ServerConfig.hpp"

std::map<int, Webserv> initMapServ(std::string file_path);
void webservInit(std::string file_path, std::map<int, Webserv> &map_serv);
std::string   show_directory_content(std::string path, char** envp);
std::string listFilesInDirectory(const std::string& path);
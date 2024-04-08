#include "utils.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/wait.h>
#include <unistd.h>

std::map<int, Webserv> initMapServ(std::string file_path)
{
	std::map<int, Webserv> res;

	std::ifstream file(file_path.c_str());
	if (!file)
		throw std::runtime_error("Cannot open file: " + file_path);

	std::string line;
    std::string current_location;

	ServerConfig actual_config;

	while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string word;
        iss >> word;

        if (word == "server") {
            // Début d'un bloc de serveur
        } else if (word == "listen") {
			int _port;
            iss >> _port;
			actual_config.setPort(_port);
        } else if (word == "server_name") {
			std::string _server_name;
            iss >> _server_name;
			if (_server_name[_server_name.size() - 1] == ';')
				_server_name.erase(_server_name.size() - 1);
			actual_config.setServerName(_server_name);
        } else if (word == "location") {
            iss >> current_location;
        } else if (word == "root" || word == "access" || word == "method") {
            std::string value;
            iss >> value;
			if (value[value.size() - 1] == ';')
				value.erase(value.size() - 1);
			actual_config.setLocationValue(current_location, word, value);
        } else if (word == "}") {
            if (!current_location.empty())
                current_location.clear();
			else {
				Webserv server(actual_config); // Nouveau serveur avec la nouvelle configuration
				res[server.getFd()] = server; // Ajout du serveur à la map
				std::cout << "New server: " << actual_config.getServerName() << std::endl; // On le dit
				actual_config = ServerConfig(); // On réinitialise la configuration pour le prochain serveur
			}
	    }
    }

	file.close();
    return res;
}
std::string read_from_pipe(int fd)
{
    const int   buffer_size = 4096;
    char        buffer[buffer_size];
    std::string result;
    ssize_t     bytes_read;

    while (true)
    {
        bytes_read = read(fd, buffer, buffer_size);
        if (bytes_read == -1)
        {
            perror("Error reading pipe out:");
            exit(EXIT_FAILURE);
        }
        if (bytes_read > 0)
            result.append(buffer, bytes_read);
        if (bytes_read < buffer_size)
            break;
    }
    return result;
}

void webservInit(std::string file_path, std::map<int, Webserv> &map_serv)
{
	try
	{
		map_serv = initMapServ(file_path);
	}
	catch (std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		exit(1);
	}
}

std::string listFilesInDirectory(const std::string& path)
{
    DIR* dir = opendir(path.c_str());
    if (dir == NULL)
    {
        // Le répertoire n'a pas pu être ouvert
        return "";
    }

    std::string files;
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL)
    {
        // Ne pas inclure les entrées "." et ".."
        if (std::string(entry->d_name) != "." && std::string(entry->d_name) != "..")
        {
            files += std::string(entry->d_name) + "\n";
        }
    }

    closedir(dir);
    return files;
}


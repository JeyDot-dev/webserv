# Webserv: a simple HTTP Server

This is a custom HTTP 1.1 server implemented in C++98, designed to handle multiple simultaneous clients using `select()`. The server supports opening multiple ports to host multiple sites and is capable of handling GET, POST, and DELETE HTTP methods. It includes a configuration file handler inspired by nginx configuration files and supports executing PHP and Python CGI scripts.

## Features

- **HTTP Protocol**: Based on HTTP 1.1 specifications.
- **Client Handling**: Utilizes `select()` for managing multiple clients concurrently.
- **Multi-site Hosting**: Capable of opening multiple ports to host multiple sites simultaneously.
- **HTTP Methods**: Handles GET, POST, and DELETE HTTP methods.
- **Configuration File**: Provides a configuration file handler similar to nginx for server setup.
- **CGI Support**: Executes PHP and Python CGI scripts for dynamic content generation.

## Requirements

- C++98 compatible compiler (e.g., GCC)
- Standard C++ Library

## Usage

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/JeyDot-dev/webserv.git
   ```

2. **Build the Server:**
   ```bash
   cd webserv
   make
   ```

3. **Run the Server:**
   ```bash
   ./webserv [optional config file (server.conf by default)]
   ```

4. **Configuration:**
   - Modify `server.conf` to configure server settings and define hosted sites.


Made by LeBojo and I

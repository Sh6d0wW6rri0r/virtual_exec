/*
This program has been created by Olivier Moulin and is distributed under
the GNU public license.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef READSEND
#define READSEND



#ifndef WIN32

#include <netdb.h>
#include <ctype.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#endif

#ifdef WIN32
  #include <windows.h>
  #include <winsock.h>
#endif

#include "Commun/Definition/VEDefinition.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


VECLASS NetworkServer {
private :
	int Port;
	int socketd;
	int clientsocket;
public :
	NetworkServer();
	void init_server (int port);
	void wait_connection();
	int is_ready();
	void close_connection();
	char * read_line();
	void send_line(char * st);
};


VECLASS NetworkClient {
private :
	int Port;
	char * Server;
	int socketd;
public :
	NetworkClient();
	void init_client (char * serv,int port);
	void connect_client();
	int is_connected();
	void close_connection();
	char * read_line();
	void send_line(char * st);
};



#endif

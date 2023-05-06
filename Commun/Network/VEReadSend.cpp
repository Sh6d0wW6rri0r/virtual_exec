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
#include "Commun/Network/VEReadSend.h"


NetworkServer::NetworkServer() {
  socketd=-1;
}

void NetworkServer::init_server (int port) {
  struct  sockaddr_in sad; /* structure to hold server's address  */
  int     sd;
  #ifndef WIN32
  socklen_t     alen;            /* length of address                   */
  #endif
  #ifdef WIN32
  WSADATA wsaData;
  WSAStartup(0x0101, &wsaData);
  #endif
  Port = port;
  memset((char *)&sad,0,sizeof(sad)); /* clear sockaddr structure */
  sad.sin_family = AF_INET;         /* set family to Internet     */
  sad.sin_addr.s_addr = INADDR_ANY; /* set the local IP address   */
  sad.sin_port = htons((u_short)port);
  sd = socket(AF_INET, SOCK_STREAM, 0);
  if (sd < 0) {
    printf("Error creating Socket ...");
    socketd = -1;
	return;
  }
  if (bind(sd, (struct sockaddr *)&sad, sizeof(sad)) < 0) {
    printf("Error binding Socket ...");
	socketd = -1;
    return;
  }
  if (listen(sd, 10) < 0) {
    printf("Error listening Socket ...");
	socketd=-1;
    return;
  }
  socketd=sd;
}

void NetworkServer::wait_connection() {
  struct sockaddr_in cad;
  #ifndef WIN32
  socklen_t alen;
  #endif
  #ifdef WIN32
  int alen;
  alen=sizeof(cad);
  #endif
  clientsocket=accept(socketd, (struct sockaddr *)&cad, &alen);
}

int NetworkServer::is_ready() {
	if (socketd==-1) {
		return 0;
	}
	else {
		return 1;
	}
}

void NetworkServer::close_connection() {
  #ifndef WIN32
  close(clientsocket);
  #endif
  #ifdef WIN32
  closesocket(clientsocket);
  #endif
}

char * NetworkServer::read_line() {
  char * res = new char[255];
  char * c = new char[2];
  int ret;
  int done =0;
  int cpt=0;
  while (done==0) {
    ret=recv(clientsocket,c,1,0);
    if (c[0]=='\n' || ret<=0) {
      done=1;
    }
    else {
      res[cpt]=c[0];
      res[cpt+1]=0;
      cpt++;
    }
  }
  if (ret<=0) {
    strcpy(res,"DISC");
  }
  return res;
}

void NetworkServer::send_line(char * st) {
  char * endt= new char[strlen(st)+2];
  sprintf(endt,"%s%c",st,'\n');
  send(clientsocket,endt,strlen(endt),0);
}


NetworkClient::NetworkClient() {
	socketd=-1;
}

void NetworkClient::init_client (char * serv,int port) {
  Server = new char[strlen(serv)];
  strcpy(Server,serv);
  Port=port;
}

void NetworkClient::connect_client() {
  struct  hostent  *ptrh;  /* pointer to a host table entry       */
  struct  sockaddr_in sad; /* structure to hold an IP address     */
  int SocketDesc;
  #ifdef WIN32
  WSADATA wsaData;
  WSAStartup(0x0101, &wsaData);
  #endif

  memset((char *)&sad,0,sizeof(sad)); /* clear sockaddr structure */
  sad.sin_family = AF_INET;         /* set family to Internet     */
  sad.sin_port = htons((u_short)Port);
  ptrh = gethostbyname(Server);

  memcpy(&sad.sin_addr, ptrh->h_addr, ptrh->h_length);
  SocketDesc = socket(AF_INET, SOCK_STREAM,0);
  if (SocketDesc < 0) {
    socketd=-1;
    return;
  }
  if (connect(SocketDesc, (struct sockaddr *)&sad, sizeof(sad)) < 0) {
      socketd=-1;
	  return;
  }
  socketd=SocketDesc;
}

int NetworkClient::is_connected() {
	if (socketd==-1) {
		return 0;
	}
	else {
		return 1;
	}
}

void NetworkClient::close_connection(){
  #ifndef WIN32
  close(socketd);
  #endif
  #ifdef WIN32
  closesocket(socketd);
  #endif
}

char * NetworkClient::read_line(){
  char * res = new char[255];
  char * c = new char[2];
  int ret;
  int done =0;
  int cpt=0;
  while (done==0) {
    ret=recv(socketd,c,1,0);
    if (c[0]=='\n' || ret<=0) {
      done=1;
    }
    else {
      res[cpt]=c[0];
      res[cpt+1]=0;
      cpt++;
    }
  }
  if (ret<=0) {
    strcpy(res,"DISC");
  }
  return res;
}

void NetworkClient::send_line(char * st) {
  char * endt= new char[strlen(st)+2];
  sprintf(endt,"%s%c",st,'\n');
  send(socketd,endt,strlen(endt),0);
}

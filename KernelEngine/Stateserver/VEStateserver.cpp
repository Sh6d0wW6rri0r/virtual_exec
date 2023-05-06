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

#include "KernelEngine/Engine/VEEngine.h"
#include "KernelEngine/Stateserver/VEStateserver.h"

void *stateserver(void * ptr) {
  int SocketDesc;
  int     sd;
  char * sendbuf= new char[255];
  char * buf= new char[255];
  unsigned int i;
  VEEngine * connectedengine=(VEEngine*)ptr;
  NetworkServer * state_server = new NetworkServer();


  state_server->init_server(9999);
  while (1) {
    fflush(NULL);
    state_server->wait_connection();
    int done=0;
    while (done==0) {
      strcpy(buf,state_server->read_line());
      if (strcmp(buf,"GET_TOTALMEM")==0) {
         sprintf(sendbuf,"%d",connectedengine->GetMemoryMax());
         state_server->send_line(sendbuf);
      }
      if (strcmp(buf,"GET_FREEMEM")==0) {
        sprintf(sendbuf,"%d",connectedengine->GetMemoryFree());
        state_server->send_line(sendbuf);
      }
      if (strcmp(buf,"DISC")==0) {
        done=1;
      }
    }
    state_server->close_connection();
  }
  delete buf;
  delete sendbuf;
  return NULL;
}

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

#include "KernelMemory/RemoteCard/VERemoteCard.h"

VERemoteCard::VERemoteCard(){
	ns = new NetworkServer();
}

void VERemoteCard::SetPort(int prt) {
  Port=prt;
}

void VERemoteCard::Init(unsigned int mmax) {
  char * sendbuf= new char[255];
  char * buf= new char[255];
  unsigned int i;

  space = new unsigned int[mmax];
  for (i = 0;i < mmax; i++) {
    space[i] = 0;
  }
  memmax = mmax;

  printf("\nRemote Memory Card for Virtual Exec (c) O.Moulin");
  printf("\nMemory Size : %d\n\n",GetMemoryMax());
  ns->init_server(Port);
  while (1) {
    fflush(NULL);
#ifdef DEBUG
    printf("\nREMMEM >> Waiting for connection ...");
#endif
	ns->wait_connection();
    int done=0;
#ifdef DEBUG
      printf("\nREMMEM >> Connected to a Virtual Machine");
#endif
    while (done==0) {
      strcpy(buf,ns->read_line());
      if (strcmp(buf,"GET_MEMMAX")==0) {
        sprintf(sendbuf,"%d",GetMemoryMax());
        ns->send_line(sendbuf);
#ifdef DEBUG
          printf("\nREMMEM >> Sending the amount of Memory ...");
#endif
      }
      if (strcmp(buf,"GET_ADDR")==0) {
         strcpy(buf,ns->read_line());
         sprintf(sendbuf,"%d",GetAddress(atol(buf))->GetValue());
         ns->send_line(sendbuf);
#ifdef DEBUG
           printf("\nREMMEM >> Sending the Memory Value ...");
#endif
      }
      if (strcmp(buf,"STORE_ADDR")==0) {
        char * buf2 = new char[255];
        strcpy(buf,ns->read_line());
        strcpy(buf2,ns->read_line());
        VEMemSpace * t = new VEMemSpace();
        t->SetValue(atoi(buf2));
        StoreAddress(atoi(buf),t);
#ifdef DEBUG
          printf("\nREMMEM >> Storing Value into Memory ...");
#endif
      }
      if (strcmp(buf,"DISC")==0) {
        done=1;
#ifdef DEBUG
          printf("\nREMMEM >> Disconnecting from the Virtual Machine ...");
#endif
      }
    }
    ns->close_connection();
  }
  delete buf;
  delete sendbuf;
  LocalShutdown();
}

unsigned int VERemoteCard::GetMemoryMax() {
  return memmax;
}

VEMemSpace * VERemoteCard::GetAddress(unsigned int ma) {
  VEMemSpace * tmp = new VEMemSpace();
  tmp->SetValue(space[ma]);
  return tmp;
}

void VERemoteCard::StoreAddress(unsigned int ma,VEMemSpace * val) {
  space[ma] = val->GetValue();
}

void VERemoteCard::LocalShutdown() {
}

VERemoteCard::~VERemoteCard() {
  delete space;
}

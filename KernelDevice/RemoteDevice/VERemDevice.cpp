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

#include "KernelDevice/RemoteDevice/VERemDevice.h"

// -------------------------------------------
// Definition des methodes de la classe SERVDEVICE
// -------------------------------------------

VERemoteDevice::VERemoteDevice() {
  ns = new NetworkServer();
}

void VERemoteDevice::Stop() {
}

void VERemoteDevice::Start() {
}

void VERemoteDevice::Shutdown() {
}

void VERemoteDevice::LinkDevice(VEDevice * d) {
  linkeddevice=d;
}


void VERemoteDevice::SetPort (int prt) {
  Port=prt;
}


void VERemoteDevice::Init() {
  int     sd;         /* socket descriptors                  */
  char * sendbuf= new char[255];
  char * buf= new char[255];


  linkmem = new VERemMControler();
  linkreg = new VERemRControler();
  linkeddevice->Init(linkmem);
  printf("\nRemote Device for Virtual Exec (c) O.Moulin");
  printf("\nDevice : %s\n\n",linkeddevice->GetDescriptor());
  ns->init_server(Port);
  while (1) {
    fflush(NULL);
#ifdef DEBUG
      printf("\nREMDEV >> Waiting for connection ...");
#endif
    ns->wait_connection();
    int done=0;
#ifdef DEBUG
      printf("\nREMDEV >> Connected to a Virtual Machine");
#endif
    linkmem->SetSocket(ns);
    linkreg->SetSocket(ns);
    while (done==0) {
      strcpy(buf,ns->read_line());
      if (strcmp(buf,"GET_DESC")==0) {
        strcpy(sendbuf,linkeddevice->GetDescriptor());
        ns->send_line(sendbuf);
#ifdef DEBUG
          printf("\nREMDEV >> Sending the descriptor ...");
#endif
      }
      if (strcmp(buf,"GET_DEVID")==0) {
        sprintf(sendbuf,"%ld",linkeddevice->GetDeviceID());
        ns->send_line(sendbuf);
#ifdef DEBUG
          printf("\nREMDEV >> Sending the ID ...");
#endif
      }
      if (strcmp(buf,"GET_MSTART")==0) {
        sprintf(sendbuf,"%ld",linkeddevice->GetMemoryStart());
        ns->send_line(sendbuf);
#ifdef DEBUG
          printf("\nREMDEV >> Sending the Memory Start point ...");
#endif
      }
      if (strcmp(buf,"GET_MSPACE")==0) {
        sprintf(sendbuf,"%ld",linkeddevice->GetMemorySpace());
        ns->send_line(sendbuf);
#ifdef DEBUG
          printf("\nREMDEV >> Sending the Memory length ...");
#endif
      }
      if (strcmp(buf,"GET_MAXINT")==0) {
        sprintf(sendbuf,"%ld",linkeddevice->GetMaxInterrupt());
        ns->send_line(sendbuf);
#ifdef DEBUG
          printf("\nREMDEV >> Sending the number of occupied interrupt ...");
#endif
      }
      if (strcmp(buf,"GET_INT")==0) {
        for (unsigned char k=0;k<linkeddevice->GetMaxInterrupt();k++) {
          sprintf(sendbuf,"%ld",linkeddevice->GetInterrupt(k));
          ns->send_line(sendbuf);
        }
#ifdef DEBUG
          printf("\nREMDEV >> Sending the occupied interrupts ...");
#endif
      }
      if (strcmp(buf,"START")==0) {
        linkeddevice->Start();
		#ifdef DEBUG
          printf("\nREMDEV >> Enabling device ...");
		#endif
      }
      if (strcmp(buf,"STOP")==0) {
        linkeddevice->Stop();
      }
      if (strcmp(buf,"EXEC")==0) {
#ifdef DEBUG
          printf("\nREMDEV >> Executing Interrupt ...");
#endif
        linkeddevice->ExecInterrupt((VERControler *)linkreg);
        strcpy(sendbuf,"END_EXEC");
        ns->send_line(sendbuf);
#ifdef DEBUG
          printf("\nREMDEV >> Executing Interrupt finished");
#endif
      }
      if (strcmp(buf,"DISC")==0) {
        done=1;
#ifdef DEBUG
          printf("\nREMDEV >> Disconnecting from the Virtual Machine ...");
#endif
      }
    }
    ns->close_connection();
  }
  delete buf;
  delete sendbuf;
}


VERemoteDevice::~VERemoteDevice() {
}

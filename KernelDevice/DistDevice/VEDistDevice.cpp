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

#include "KernelDevice/DistDevice/VEDistDevice.h"

// -------------------------------------------
// Definition des methodes de la classe DISTDEVICE
// -------------------------------------------

VEDistDevice::VEDistDevice() {
}

void VEDistDevice::SetServer (char * serv,int prt) {
  nc = new NetworkClient();
  nc->init_client(serv,prt);
}

void VEDistDevice::LocalInit() {
  char    *buf= new char[255];
  char    *sendbuf = new char[255];
  char    *cmd = new char[255];
  char    *buftmp = new char[255];

  nc->connect_client();
#ifdef DEBUG
    printf("\nGetting Remote Device Description ...");
#endif
  strcpy(sendbuf,"GET_DESC");
  nc->send_line(sendbuf);
  strcpy(buf,nc->read_line());
  strcpy(descriptor,buf);
  strcpy(sendbuf,"GET_DEVID");
  nc->send_line(sendbuf);
  strcpy(buf,nc->read_line());
  deviceID = atoi(buf);
  strcpy(sendbuf,"GET_MSTART");
  nc->send_line(sendbuf);
  strcpy(buf,nc->read_line());
  memoryStart=atoi(buf);
  strcpy(sendbuf,"GET_MSPACE");
  nc->send_line(sendbuf);
  strcpy(buf,nc->read_line());
  memorySpace=atoi(buf);
  strcpy(sendbuf,"GET_MAXINT");
  nc->send_line(sendbuf);
  strcpy(buf,nc->read_line());
  maxinterrupt = atoi(buf);
  interrupt = new unsigned char[maxinterrupt];
#ifdef DEBUG
    printf("\nDISDEV >> Getting Remote Device Interrupt List ...");
#endif
  strcpy(sendbuf,"GET_INT");
  nc->send_line(sendbuf);
  for (int j=0;j<maxinterrupt;j++) {
    strcpy(buf,nc->read_line());
    interrupt[j]=atoi(buf);
  }
  delete buf;
  delete sendbuf;
  delete cmd;
  delete buftmp;
}

void VEDistDevice::LocalStart() {
  char * sendbuf = new char[255];

  strcpy(sendbuf,"START");
  nc->send_line(sendbuf);
  delete sendbuf;
}

void VEDistDevice::LocalStop() {
  char * sendbuf = new char[255];

  strcpy(sendbuf,"STOP");
  nc->send_line(sendbuf);
  delete sendbuf;
}

void VEDistDevice::ExecInterrupt(VERControler * lr) {
  char    *buf= new char[255];
  char    *sendbuf = new char[255];

  linkreg=lr;
  strcpy(sendbuf,"EXEC");
  nc->send_line(sendbuf);
  strcpy(buf,"EXEC");
  while (strcmp(buf,"END_EXEC")!=0) {
    strcpy(buf,nc->read_line());
#ifdef DEBUG
      printf("\nDISDEV >> REM-CMD : %s",buf);
#endif
    if (strcmp(buf,"GET_REG")==0) {
      strcpy(buf,nc->read_line());
      unsigned int val=linkreg->GetRegister(atoi(buf))->GetValue();
      sprintf(sendbuf,"%d",val);
      nc->send_line(sendbuf);
#ifdef DEBUG
        printf("\nDISDEV >> Sending Register value ...");
#endif
    }
    if (strcmp(buf,"SET_REG")==0) {
      strcpy(buf,nc->read_line());
      unsigned int val=atol(buf);
      strcpy(buf,nc->read_line());
      linkreg->SetRegister(val,atoi(buf));
#ifdef DEBUG
        printf("\nDISDEV >> Setting Register value ...");
#endif
    }
    if (strcmp(buf,"GET_MEM")==0) {
      strcpy(buf,nc->read_line());
      unsigned int val=linkmem->GetAddress((unsigned int)atoi(buf))->GetValue();
      sprintf(sendbuf,"%d",val);
      nc->send_line(sendbuf);
#ifdef DEBUG
        printf("\nDISDEV >> Sending Memory value ...");
#endif
    }
    if (strcmp(buf,"STR_MEM")==0) {
      char buftmp[255];
      strcpy(buftmp,nc->read_line());
      strcpy(buf,nc->read_line());
      VEMemSpace * tmp = new VEMemSpace();
      tmp->SetValue(atoi(buf));
      linkmem->StoreAddress((unsigned int)atoi(buftmp),tmp);
#ifdef DEBUG
      printf("\nDISDEV >> Storing to Memory ...");
#endif
    }
  }
  delete buf;
  delete sendbuf;
}

void VEDistDevice::LocalShutdown() {
  char * sendbuf = new char[255];
  strcpy(sendbuf,"DISC");
  nc->send_line(sendbuf);
  VESLEEP(1);
  nc->close_connection();
  delete sendbuf;
}

VEDistDevice::~VEDistDevice() {
}

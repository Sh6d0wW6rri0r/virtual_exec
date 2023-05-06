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

#include "KernelDevice/RemoteDevice/RemRControler/VERemRControler.h"

VERemRControler::VERemRControler() {
}

void VERemRControler::Init() {
}

void VERemRControler::SetSocket(NetworkServer * sd) {
  ns = sd;
}

VEMemSpace * VERemRControler::GetRegister (unsigned char nreg) {
  char    *buf= new char[255];
  char    *sendbuf = new char[255];

  strcpy(sendbuf,"GET_REG");
  ns->send_line(sendbuf);
  sprintf(sendbuf,"%ld",nreg);
  ns->send_line(sendbuf);
  strcpy(buf,ns->read_line());
  VEMemSpace * tmp = new VEMemSpace();
  tmp->SetValue(atol(buf));
#ifdef DEBUG
    printf("\nREMSHR >> Getting Remote Register Value ...");
#endif
  delete buf;
  delete sendbuf;
  return(tmp);
}

void VERemRControler::SetRegister (unsigned char nreg,VEMemSpace * val) {
  char    *buf= new char[255];
  char    *sendbuf = new char[255];

  strcpy(sendbuf,"SET_REG");
  ns->send_line(sendbuf);
  sprintf(sendbuf,"%ld",nreg);
  ns->send_line(sendbuf);
  sprintf(sendbuf,"%ld",val->GetValue());
  ns->send_line(sendbuf);
#ifdef DEBUG
    printf("\nREMSHR >> Setting Remote Register Value ...");
#endif
  delete buf;
  delete sendbuf;
}

void VERemRControler::SetRegister (unsigned char nreg,unsigned long val) {
  char    *buf= new char[255];
  char    *sendbuf = new char[255];

  strcpy(sendbuf,"SET_REG");
  ns->send_line(sendbuf);
  sprintf(sendbuf,"%ld",nreg);
  ns->send_line(sendbuf);
  sprintf(sendbuf,"%ld",val);
  ns->send_line(sendbuf);
#ifdef DEBUG
    printf("\nREMSHR >> Setting Remote Register Value ...");
#endif
  delete buf;
  delete sendbuf;
}

void VERemRControler::Shutdown() {
}

VERemRControler::~VERemRControler() {
}

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

#include "KernelDevice/RemoteDevice/RemMControler/VERemMControler.h"

VERemMControler::VERemMControler() {
}

void VERemMControler::Init() {
}

void VERemMControler::SetSocket(NetworkServer * sd) {
  ns = sd;
}

VEMemSpace * VERemMControler::GetAddress(unsigned int add) {
  char    *buf= new char[255];
  char    *sendbuf = new char[255];

  strcpy(sendbuf,"GET_MEM");
  ns->send_line(sendbuf);
  sprintf(sendbuf,"%d",add);
  ns->send_line(sendbuf);
  strcpy(buf,ns->read_line());
  VEMemSpace * tmp = new VEMemSpace();
  tmp->SetValue(atoi(buf));
#ifdef DEBUG
    printf("\nREMSHM >> Getting Remote Address Value ...");
#endif
  delete buf;
  delete sendbuf;
  return(tmp);
}

void VERemMControler::StoreAddress(unsigned int add,VEMemSpace * val) {
  char * sendbuf = new char[255];
  strcpy(sendbuf,"STR_MEM");
  ns->send_line(sendbuf);
  sprintf(sendbuf,"%d",add);
  ns->send_line(sendbuf);
  sprintf(sendbuf,"%d",val->GetValue());
  ns->send_line(sendbuf);
#ifdef DEBUG
  printf("\nREMSHM >> Storing to remote memory ...");
#endif
  delete sendbuf;
}

void VERemMControler::Shutdown() {
}

VERemMControler::~VERemMControler() {
}

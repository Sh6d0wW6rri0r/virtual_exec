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

#include "KernelMemory/DistMemory/VEDistMemory.h"

VEDistMemory::VEDistMemory() {
}

void VEDistMemory::SetServer (char * serv,int prt) {
  nc = new NetworkClient();
  nc->init_client(serv,prt);
}

void VEDistMemory::Init() {
  nc->connect_client();
}

unsigned int  VEDistMemory::GetMemoryMax() {
  unsigned int  memmax;
  char * sendbuf = new char[255];
  char * buf = new char[255];
  strcpy(sendbuf,"GET_MEMMAX");
  nc->send_line(sendbuf);
  strcpy(buf,nc->read_line());
  memmax=atoi(buf);
  delete sendbuf;
  delete buf;
  return memmax;
}

VEMemSpace * VEDistMemory::GetAddress(unsigned int ma) {
  VEMemSpace * res = new VEMemSpace();
  char * sendbuf = new char[255];
  char * buf = new char[255];
  strcpy(sendbuf,"GET_ADDR");
  nc->send_line(sendbuf);
  sprintf(sendbuf,"%d",ma);
  nc->send_line(sendbuf);
  strcpy(buf,nc->read_line());
  res->SetValue(atoi(buf));
  delete sendbuf;
  delete buf;
  return res;
}


void VEDistMemory::StoreAddress(unsigned int ma,VEMemSpace * val) {
  char * sendbuf = new char[255];
  char * buf = new char[255];

  strcpy(sendbuf,"STORE_ADDR");
  nc->send_line(sendbuf);
  sprintf(sendbuf,"%d",ma);
  nc->send_line(sendbuf);
  sprintf(sendbuf,"%d",val->GetValue());
  nc->send_line(sendbuf);
  delete sendbuf;
  delete buf;
}

void VEDistMemory::LocalShutdown() {
  char * sendbuf = new char[255];

  strcpy(sendbuf,"DISC");
  nc->send_line(sendbuf);
  VESLEEP(1);
  nc->close_connection();
  delete sendbuf;
}

VEDistMemory::~VEDistMemory() {
}

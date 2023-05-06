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

// -------------------------------------------
// Definition des methodes de la classe ENGINE
// -------------------------------------------

VEEngine::VEEngine() {
	nc = new NetworkClient();
}

void VEEngine::Init(unsigned int stacksize,VEProcessor * pr) {
  Trace("--");
  Trace("Virtual Exec Engine (c) O.Moulin");
  Trace("Looking for environment ...");
  Trace("Initializing Memory Card Plug and Play table ...");
  connectedmemory = new VEMemCtrl();
  connectedmemory->Init();
  VEMemoryCard * boot = new VEMemoryCard();
  boot->Init(2);
  AddMemoryCard(boot);
  Trace("Memory Card Plug and Play table initialized");
  connectedstack = new VEStack();
  connectedstack->Init(stacksize);
  char tmptr[255];
  sprintf(tmptr,"  %d %s",stacksize,"Words of stack initialized");
  Trace(tmptr);
  connectedinterrupt = new VEInterrupt();
  connectedinterrupt->Init();
  Trace("  255 interrupts table initialized");
  Trace("Initializing Devices Plug and Play table...");
  connecteddevice= new VEDevCtrl();
  connecteddevice->Init();
  Trace("Devices Plug and Play table initialized");
  VEMemSpace * dat = new VEMemSpace();
  dat->SetValue(254);
  connectedmemory->StoreAddress(0,dat);
  connectedprocessor=new VEProcCtrl();
  Trace("Initializing Central Processor ...");

  connectedprocessor->Init(connectedmemory,connectedstack,connectedinterrupt,connecteddevice);
  connectedprocessor->AddProcessor(pr);
  Trace(pr->GetDescriptor());
  Trace("Central Processor Initialized");

}

void VEEngine::AddProcessor(VEProcessor *p) {
  Trace("Connecting New Processor");
  connectedprocessor->AddProcessor(p);
  Trace(p->GetDescriptor());
  Trace("New Processor connected");
}


void VEEngine::ConnectSupervisor(char * server,int port) {
  char    *buf= new char[255];
  char    *sendbuf = new char[255];


  nc->init_client(server,port);

  nc->connect_client();
  strcpy(sendbuf,"DISP_INFO");
  nc->send_line(sendbuf);
  strcpy(sendbuf,"Begin trace on the supervisor");
  nc->send_line(sendbuf);
}

void VEEngine::Trace(char * s) {
  char sendbuf[255];
  if (nc->is_connected()==0) {
    printf("\n %s",s);
    fflush(stdout);
  }
  else {
    strcpy(sendbuf,"DISP_INFO");
    nc->send_line(sendbuf);
    nc->send_line(s);
  }
}

void VEEngine::AddMemoryCard(VEMemory * mc){
  Trace("Plugging of a new memory card and initializing ...");
  connectedmemory->AddMemory(mc);
  char tmptr[255];
  sprintf(tmptr,"%ld %s",connectedmemory->GetMemoryMax(),"Words of memory initialized");
  Trace(tmptr);
  Trace("New memory card connected");
}

unsigned int VEEngine::GetMemoryMax() {
  return connectedmemory->GetMemoryMax();
}

unsigned int VEEngine::GetMemoryFree() {
  return 0;
}

void VEEngine::AddDevice(VEDevice * d) {
  Trace("Plugging of a new device and initializing ...");
  d->Init(connectedmemory);
  connecteddevice->AddDevice(d);
  char tmptr[255];
  sprintf(tmptr,"  Device added : %s ",d->GetDescriptor());
  Trace(tmptr);
  int j;
  Trace("Listing occupied interrupt ...");
  for (j=0;j<d->GetMaxInterrupt();j++) {
#ifdef DEBUG
      sprintf(tmptr,"\nENGINE >>  INTR : %u DEVID : %d ",d->GetInterrupt(j),d->GetDeviceID());
      Trace(tmptr);
#endif
    connectedinterrupt->SetDevice(d->GetInterrupt(j),d->GetDeviceID());
  }
  Trace("New device connected");
}

void VEEngine::DelDevice(VEDevice * d) {
  connecteddevice->DelDevice(d);
}

void VEEngine::LoadFile(char * filename) {
  unsigned int opcode;
  unsigned int cptmem=0;
  VEMemSpace * m = new VEMemSpace();
  Trace("Reading program file ...");
  std::ifstream finput(filename, std::ios::binary);
  if (!finput) {
	Trace("unable to open program file. \n");
    exit(1);
  }
  while (finput) {
    finput.read((char*)&opcode, sizeof(unsigned int));
    m->SetValue(opcode);
#ifdef DEBUG
      char tmptr[255];
      sprintf(tmptr,"ENGINE >> Writing ... OP : %u MEMPOS : %u",opcode,cptmem);
      Trace(tmptr);
#endif
    connectedmemory->StoreAddress(cptmem,m);
    cptmem++;
  }
  finput.close();
  m->SetLow(254);
  m->SetHigh(0);
  connectedmemory->StoreAddress(cptmem,m);
  m->SetLow(254);
  m->SetHigh(0);
  connectedmemory->StoreAddress(cptmem + 1, m);
  //delete m;
  Trace("Program file loaded");
}


void VEEngine::StartEngine() {
  unsigned char currentop=0;
  Trace("Booting ...");
  connectedprocessor->Start();
}

void VEEngine::StopEngine() {
  Trace("Shuting Down ...");
  connectedprocessor->Shutdown();
  connecteddevice->Shutdown();
  connectedmemory->Shutdown();
  connectedstack->Shutdown();
  Trace("Shut Down completed");
  Trace("--");
}

void VEEngine::Clear() {
}

VEEngine::~VEEngine() {
  delete connectedmemory;
  delete connectedstack;
  delete connectedinterrupt;
  delete connecteddevice;
}

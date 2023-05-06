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

#include "KernelProcessor/ProcControler/VEProcCtrl.h"


VEProcCtrl::VEProcCtrl() {
}

void VEProcCtrl::Init(VEMemCtrl * mc,VEStack * sc,VEInterrupt * ic,VEDevCtrl * dc) {
  linkedmemory=mc;
  linkedstack=sc;
  linkedinterrupt=ic;
  linkeddevice=dc;
  tabproc=new VEProcessor*[255];
  for (int i=0;i<255;i++) {
    tabproc[i]=NULL;
  }
  maxproc=0;
}

void VEProcCtrl::Start() {
    startingproc = 0;
    startingaddr = 0;
    this->RunProcessor();
}

void * RuntimeProc(void * param) {
  VEProcCtrl * tmp = (VEProcCtrl * )param;
  tmp->RunProcessor();
   return NULL;
}

void VEProcCtrl::RunProcessor() {
  tabproc[startingproc]->Execute(startingaddr);
}


void VEProcCtrl::StartProcessor(int proc,unsigned int addr) {
  VETHREAD prcth;
  startingproc=proc;
  startingaddr=addr;
  this->RunProcessor();
  //prcth=launch_thread(&RuntimeProc,(void *)this);
}

void VEProcCtrl::StopProcessor(int proc) {
  tabproc[proc]->Halt();
}

void VEProcCtrl::StartNextProcessor(unsigned int addr) {
  unsigned int i=0;
  int done=1;
  while (i<maxproc && done==0) {
    if (tabproc[i]->Active()==false) {
      done=1;
    }
    else {
        i++;
    }
  }
  if (done==1) {
    StartProcessor(i,addr);
  }
  else {
    printf("PROCESSOR >> No unactive processor available ...");
  }
}

void VEProcCtrl::AddProcessor(VEProcessor * p) {
  p->Init(linkedmemory,linkedstack,linkedinterrupt,linkeddevice);
  p->LinkProcCtrl(this);
  tabproc[maxproc]=p;
  maxproc++;
}

void VEProcCtrl::Stop() {
  for (unsigned int i=0;i<maxproc;i++) {
    if (tabproc[i]!=NULL) {
       tabproc[i]->Halt();
    }
  }
}

void VEProcCtrl::Shutdown() {
  for (unsigned int i=0;i<maxproc;i++) {
    if (tabproc[i]!=NULL) {
      tabproc[i]->Shutdown();
    }
  }
}

VEProcCtrl::~VEProcCtrl() {
}

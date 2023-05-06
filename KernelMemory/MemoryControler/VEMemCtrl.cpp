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

#include "KernelMemory/MemoryControler/VEMemCtrl.h"

VEMemCtrl::VEMemCtrl() {
}

void VEMemCtrl::Init() {
 unsigned int i;
 bus = new VEMemory*[255];
 for (i = 0;i < 255; i++) {
   bus[i]=NULL;
 }
 memmax = 0;
 cardmax = 0;
}

void VEMemCtrl::AddMemory(VEMemory * mc) {
  bus[cardmax]=mc;
  cardmax++;
  memmax=memmax+mc->GetMemoryMax();
}

unsigned int VEMemCtrl::GetMemoryMax() {
  return memmax;
}

VEMemSpace * VEMemCtrl::GetAddress(unsigned int add) {
  unsigned int i=0;
  unsigned int mm=0;
  while (mm+bus[i]->GetMemoryMax()<add) {
    mm=mm+bus[i]->GetMemoryMax();
    i++;
  }
  unsigned int cardaddr = add-mm;
#ifdef DEBUG
    printf("\nMEMCTRL >> Reading at %d on card %d",cardaddr,i);
#endif
  return bus[i]->GetAddress(cardaddr);
}

void VEMemCtrl::StoreAddress(unsigned int add,VEMemSpace * val) {
  VEMemSpace * dat = new VEMemSpace();
  dat->SetValue(val->GetValue());
  unsigned int i=0;
  unsigned int mm=0;
  while (mm+bus[i]->GetMemoryMax()<add) {
    mm=mm+bus[i]->GetMemoryMax();
    i++;
  }
  unsigned int cardaddr = add-mm;
  bus[i]->StoreAddress(cardaddr,dat);
#ifdef DEBUG
    printf("\nMEMCTRL >> Stored at %ld on card %d",cardaddr,i);
#endif
}

void VEMemCtrl::Shutdown() {
  #ifdef DEBUG
    printf("\nMEMCTRL >> Memory ShutDown");
  #endif
}

VEMemCtrl::~VEMemCtrl() {
  for (unsigned int i=0;i<cardmax;i++) {
    bus[i]->Shutdown();
    #ifdef DEBUG
      printf("%s %d","\nMEMCTRL >> Destroying Memory n ",i);
    #endif
    delete bus[i];
  }
  delete bus;
}

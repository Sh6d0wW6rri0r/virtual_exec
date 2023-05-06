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

#include "KernelDevice/DevCtrl/VEDevCtrl.h"


VEDevCtrl::VEDevCtrl() {
}

void VEDevCtrl::Init() {
  unsigned int i;
  devtab=new VEDevice *[255];
  for (i=0;i<255;i++) {
    devtab[i]=NULL;
  }
  maxdevice=0;
}

void VEDevCtrl::AddDevice(VEDevice * d) {
  bool ajoutefin= true;
  int i;
  if(maxdevice==0) {
    ajoutefin = true;
  }
  else {
    i=0;
    while (ajoutefin && i<maxdevice) {
      if (devtab[i]==NULL) {
        ajoutefin=false;
      }
      else {
        i++;
      }
    }
  }
  if (ajoutefin) {
    maxdevice++;
    devtab[maxdevice]=d;
  }
  else {
    devtab[i]=d;
  }
}


void VEDevCtrl::DelDevice(VEDevice * d) {
  int i=0;
  bool done = false;
  while (!done && i<maxdevice) {
    if (devtab[i]->GetDeviceID()==d->GetDeviceID()) {
      done=true;
    }
    else {
      i++;
    }
  }
  delete devtab[i];
  devtab[i]=NULL;
}

unsigned char VEDevCtrl::GetMaxDevice() {
  return maxdevice;
}

VEDevice * VEDevCtrl::GetDevice(unsigned char nbdev) {
  return devtab[nbdev];
}

VEDevice * VEDevCtrl::FindDevice(unsigned char devid) {
  VEDevice * res =NULL;
  int i=0;
  int done=0;
  while (i<255 && done==0) {
    if (devtab[i]!=NULL) {
      if (devtab[i]->GetDeviceID()==devid) {
        res=devtab[i];
        done=1;
      }
    }
    i++;
  }
  return res;
}

void VEDevCtrl::Shutdown() {
  #ifdef DEBUG
     printf("\nDEVCTRL >> Devices Shutdown");
  #endif
}

VEDevCtrl::~VEDevCtrl() {
  for (int i=0;i<255;i++) {
    if (devtab[i]!=NULL) {
      devtab[i]->Shutdown();
	  #ifdef DEBUG
        printf("%s %d","\nDEVCTRL >> Destroying Device n ",i);
      #endif
      delete devtab[i];
    }
  }
  delete devtab;
}

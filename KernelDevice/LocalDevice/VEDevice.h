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

// ----------------------------------
// Fichier entete Virtual Exec Def
// ----------------------------------

#ifndef VIRTUALEXEDEVICE
#define VIRTUALEXEDEVICE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Commun/VECommun.h"
#include "KernelDef/KernelDef.h"
#include "KernelMemory/KernelMemory.h"


// ----------------------------------------------------------------
// Classe DEVICE : classe generique de definition d un peripherique
// ----------------------------------------------------------------

VECLASS VEDevice {
  protected:
  VEMControler * linkmem;
  VERControler * linkreg;
  char descriptor[255];
  unsigned char * interrupt;
  unsigned char maxinterrupt;
  unsigned char deviceID;
  unsigned int memoryStart;
  unsigned int memorySpace;
  bool active;

  public:
  VEDevice();
  void Init(VEMControler * lm);
  virtual void LocalInit();
  char * GetDescriptor();
  unsigned char GetDeviceID();
  unsigned char GetInterrupt(unsigned char ninterrupt);
  unsigned char GetMaxInterrupt();
  unsigned long GetMemoryStart();
  unsigned long GetMemorySpace();
  virtual void ExecInterrupt(VERControler * lr);
  void Shutdown();
  virtual void LocalShutdown();
  void Start();
  void Stop();
  virtual void LocalStart();
  virtual void LocalStop();
  virtual ~VEDevice();
};

#endif

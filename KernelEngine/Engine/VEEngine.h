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
// Fichier entete Virtual Exec Engine
// ----------------------------------

#ifndef VIRTUALEXECENGINE
#define VIRTUALEXECENGINE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <string>

#include "KernelDef/KernelDef.h"
#include "KernelMemory/KernelMemory.h"
#include "KernelDevice/KernelDevice.h"
#include "Commun/VECommun.h"
#include "KernelProcessor/KernelProcessor.h"

// ----------------------------------------------------------------------
// Classe ENGINE : classe de definition du moteur de la machine virtuelle
// ----------------------------------------------------------------------


VECLASS VEEngine {

  private:
  VEDevCtrl * connecteddevice;
  VEMemCtrl * connectedmemory;
  VEStack  * connectedstack;
  VEInterrupt * connectedinterrupt;
  VEProcCtrl * connectedprocessor;
  NetworkClient * nc;

  public:
  VEEngine();
  void Init(unsigned int stacksize,VEProcessor * pr);
  VEDevCtrl * GetDeviceControler();
  VEMemCtrl * GetMemoryControler();
  VEStack * GetStack();
  VERegister * GetRegister();
  VEInterrupt * GetInterrupt();
  VEFlag * GetFlag();
  unsigned int GetMemoryMax();
  unsigned int GetMemoryFree();
  void ConnectSupervisor(char * server,int port);
  void Trace(char * s);
  void AddMemoryCard(VEMemory * mc);
  void AddDevice(VEDevice * d);
  void DelDevice(VEDevice * d);
  void AddProcessor(VEProcessor * p);
  void LoadFile(char * filename);
  void StartEngine();
  void StopEngine();
  void Clear();
  virtual ~VEEngine();
};

#endif

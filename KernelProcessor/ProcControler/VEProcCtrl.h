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

#ifndef VIRTUALEXECPROCCTRL
#define VIRTUALEXECPROCCTRL

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "KernelDef/KernelDef.h"
#include "Commun/VECommun.h"
#include "KernelMemory/KernelMemory.h"
#include "KernelDevice/KernelDevice.h"
#include "KernelProcessor/Processor/VEProcessor.h"

VECLASS VEProcCtrl {
  protected:
  VEMemCtrl * linkedmemory;
  VEDevCtrl * linkeddevice;
  VEStack * linkedstack;
  VEInterrupt * linkedinterrupt;
  VEProcessor ** tabproc;
  unsigned int maxproc;
  unsigned int startingproc;
  unsigned int startingaddr;

  public:
  VEProcCtrl();
  void Init(VEMemCtrl * mc,VEStack * sc,VEInterrupt * ic,VEDevCtrl * dc);
  void AddProcessor(VEProcessor * p);
  void RunProcessor();
  void StartProcessor(int proc,unsigned int addr);
  void StopProcessor(int proc);
  void StartNextProcessor(unsigned int addr);
  void Start();
  void Stop();
  void Shutdown();
  virtual ~VEProcCtrl();
};

#endif

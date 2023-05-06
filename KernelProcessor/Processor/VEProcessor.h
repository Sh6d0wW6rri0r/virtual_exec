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

#ifndef VIRTUALEXECPROCESSOR
#define VIRTUALEXECPROCESSOR

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "KernelDef/KernelDef.h"
#include "Commun/VECommun.h"
#include "KernelMemory/KernelMemory.h"
#include "KernelDevice/KernelDevice.h"

VECLASS VEProcCtrl;


VECLASS VEProcessor {
  protected:
  char descriptor[255];
  unsigned int id;
  char **OPCODES;

  VEProcCtrl * linkedprocctrl;
  VEMemCtrl * linkedmemory;
  VEDevCtrl * linkeddevice;
  VEStack * linkedstack;
  VEInterrupt * linkedinterrupt;
  VERegister * connectedregister;
  VEFlag * connectedflag;
  bool act;
  public:
	  VEProcessor();
  void Init(VEMemCtrl * mc,VEStack * sc,VEInterrupt * ic,VEDevCtrl * dc);
  void LinkProcCtrl(VEProcCtrl * pc);
  void StartProgramOnNextProcessor(unsigned int add);
  char * GetDescriptor();
  void MoveNextProgram();
  void MoveToProgram(unsigned int add);
  unsigned int getOpcode(const char* op);
  unsigned int GetCurrentProgram();
  VEMemSpace * GetProgram();
  unsigned int GetProcessorID();
  virtual void Execute(unsigned int add);
  void Halt();
  bool Active();
  void Shutdown();
  virtual void LocalShutdown();
  virtual void LocalInit();
};

#endif

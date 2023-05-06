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

#include "KernelProcessor/Processor/VEProcessor.h"

#include "KernelProcessor/ProcControler/VEProcCtrl.h"

VEProcessor::VEProcessor() {
    OPCODES = new char* [255];
    for (int i = 0;i < 255;i++) {
        OPCODES[i] = "NOP";
    }
}

void VEProcessor::Init(VEMemCtrl * mc,VEStack *sc,VEInterrupt * ic,VEDevCtrl *dc) {
  act=false;
  linkedmemory=mc;
  linkedstack=sc;
  linkedinterrupt=ic;
  linkeddevice=dc;
  connectedregister=new VERegister();
  connectedregister->Init();
  connectedflag = new VEFlag();
  connectedflag->Init();
  LocalInit();
}

unsigned int VEProcessor::getOpcode(const char* op) {
	unsigned int result = 255;
    int i = 0;
    int done = 0;
    while (done == 0 && i < 255) {
        if (strcmp(op, OPCODES[i]) == 0) {
            done = 1;
            result = i;
        }
        else {
            i++;
        }
    }
    return result;
}

void VEProcessor::LinkProcCtrl(VEProcCtrl * pc) {
	linkedprocctrl=pc;
}

void VEProcessor::LocalInit() {
  strcpy(descriptor,"Generic Virtual Exec Processor");
  id=0;
}

void VEProcessor::MoveNextProgram() {
  connectedregister->SetRegister(0,(connectedregister->GetRegister(0)->GetValue()+1));
}

void VEProcessor::MoveToProgram(unsigned int add) {
  connectedregister->SetRegister(0,add);
}

void VEProcessor::StartProgramOnNextProcessor(unsigned int add) {
  linkedprocctrl->StartNextProcessor(add);
}

unsigned int VEProcessor::GetCurrentProgram() {
  return connectedregister->GetRegister(0)->GetValue();
}

VEMemSpace * VEProcessor::GetProgram() {
  VEMemSpace * res = new VEMemSpace();
  res->SetValue(linkedmemory->GetAddress(connectedregister->GetRegister(0)->GetValue())->GetValue());
  return res;
}

char * VEProcessor::GetDescriptor() {
  return descriptor;
}

unsigned int VEProcessor::GetProcessorID() {
  return id;
}

void VEProcessor::Execute(unsigned int add) {
    act = true;
}


void VEProcessor::Halt() {
  act=false;
}

bool VEProcessor::Active() {
  return act;
}

void VEProcessor::Shutdown() {
  LocalShutdown();
}

void VEProcessor::LocalShutdown() {
}

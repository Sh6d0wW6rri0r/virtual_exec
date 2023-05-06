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

#include "KernelDevice/LocalDevice/VEDevice.h"

// -------------------------------------------
// Definition des methodes de la classe DEVICE
// -------------------------------------------

VEDevice::VEDevice() {
}

void VEDevice::Init(VEMControler * lm) {
  linkmem = lm;
  active=false;
  LocalInit();
}

void VEDevice::LocalInit() {
  strcpy(descriptor,"Device Generique (c) O.Moulin");
  deviceID = 0;
  maxinterrupt = 1;
  interrupt = new unsigned char[1];
  interrupt[0]=0;
  memoryStart=0;
  memorySpace=0;
}

char * VEDevice::GetDescriptor() {
  return descriptor;
}

unsigned char VEDevice::GetDeviceID() {
  return deviceID;
}

unsigned long VEDevice::GetMemoryStart() {
  return memoryStart;
}

unsigned long VEDevice::GetMemorySpace() {
  return memorySpace;
}

unsigned char VEDevice::GetInterrupt(unsigned char ninterrupt) {
  return interrupt[ninterrupt];
}

unsigned char VEDevice::GetMaxInterrupt() {
  return maxinterrupt;
}

void VEDevice::ExecInterrupt(VERControler * lr) {
  linkreg=lr;
}

void VEDevice::Stop() {
  active=false;
  LocalStop();
}

void VEDevice::Shutdown() {
  active=false;
  LocalShutdown();
}

void VEDevice::LocalShutdown() {
}

void VEDevice::Start() {
  active=true;
  LocalStart();
}

void VEDevice::LocalStart() {
}

void VEDevice::LocalStop() {
}

VEDevice::~VEDevice() {
}

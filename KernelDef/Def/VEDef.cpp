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

#include "KernelDef/Def/VEDef.h"

VEMemSpace::VEMemSpace() {
}

void VEMemSpace::SetValue(unsigned int b) {
  val.bit32=b;
}

unsigned int VEMemSpace::GetValue() {
  return val.bit32;
}

void VEMemSpace::SetLow(unsigned short int l) {
  val.bit16.low = l;
}

void VEMemSpace::SetHigh(unsigned short int h) {
  val.bit16.high = h;
}

unsigned short int VEMemSpace::GetLow() {
  return val.bit16.low;
}

unsigned short int VEMemSpace::GetHigh() {
  return val.bit16.high;
}

VEMemSpace::~VEMemSpace() {
}

VEStack::VEStack() {
}

void VEStack::Init(unsigned int smax) {
  space    = new unsigned int[smax];
  stackmax = smax;
  curptr   = 0;
}

VEMemSpace * VEStack::Pop() {
  curptr--;
  VEMemSpace * tmp = new VEMemSpace();
  tmp->SetValue(space[curptr+1]);
  return tmp;
}

void VEStack::Push(VEMemSpace * val) {
  curptr ++;
  space[curptr] = val->GetValue();
}

void VEStack::Shutdown() {
}

VEStack::~VEStack() {
	delete space;
}

VERegister::VERegister() {
}

void VERegister::Init() {
  int i;
  for (i=0;i<255;i++) {
    reg[i] = 0;
  }
}

VEMemSpace * VERControler::GetRegister (unsigned char nreg) {
  return NULL;
}

void VERControler::SetRegister (unsigned char nreg,VEMemSpace * val) {
}

void VERControler::SetRegister (unsigned char nreg,unsigned int val) {
}

void VERControler::Shutdown() {
}

VEMemSpace * VERegister::GetRegister (unsigned char nreg) {
  VEMemSpace * tmp = new VEMemSpace();
  tmp->SetValue(reg[nreg]);
  return tmp;
}

void VERegister::SetRegister (unsigned char nreg,VEMemSpace * val) {
  reg[nreg] = val->GetValue();
}

void VERegister::SetRegister (unsigned char nreg,unsigned int val) {
  reg[nreg] = val;
}

void VERegister::Shutdown() {
}

VERegister::~VERegister() {
}

VEInterrupt::VEInterrupt() {
}

void VEInterrupt::Init() {
  int i;
  for (i = 0; i < 255; i++) {
    interrupttable[i] = 0;
  }
}

unsigned char VEInterrupt::GetDevice(unsigned char interrupt) {
  return interrupttable[interrupt];
}

void VEInterrupt::SetDevice(unsigned char interrupt,unsigned char devID) {
  interrupttable[interrupt] = devID;
}

void VEInterrupt::DelDevice(unsigned char devID) {
  int i;
  for (i = 0; i < 255; i++) {
    if ( interrupttable[i] == devID) {
      interrupttable[i] = 0;
    }
  }
}

VEInterrupt::~VEInterrupt() {
}

VEFlag::VEFlag() {
}

void VEFlag::Init() {
  equalzero=false;
  divbyzero=false;
  equal=false;
  sup=false;
  inf=false;
}

bool VEFlag::IsFlagEqualZero() {
  return equalzero;
}

bool VEFlag::IsFlagDivByZero() {
  return divbyzero;
}

bool VEFlag::IsFlagEqual() {
  return equal;
}

bool VEFlag::IsFlagSup() {
  return sup;
}

bool VEFlag::IsFlagInf() {
  return inf;
}

void VEFlag::SetFlagEqualZero(bool state) {
  equalzero=state;
}

void VEFlag::SetFlagDivByZero(bool state) {
  divbyzero=state;
}

void VEFlag::SetFlagEqual(bool state) {
  equal=state;
}

void VEFlag::SetFlagSup(bool state) {
  sup=state;
}

void VEFlag::SetFlagInf(bool state) {
  inf=state;
}

VEFlag::~VEFlag() {
}

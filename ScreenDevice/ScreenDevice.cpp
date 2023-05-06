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

#include "ScreenDevice/ScreenDevice.h"

ScreenDevice::ScreenDevice() {
}

void ScreenDevice::LocalInit() {
  strcpy(descriptor,"Screen Text (c) O.Moulin");
  deviceID = 1;
  maxinterrupt = 1;
  interrupt = new unsigned char[1];
  interrupt[0]=1;
}

// The Text Screen Driver is linked on the 1 interrupt
//
// The register 2 give the choice beetween the functions called for this device
// The register 3,4,5,6,7,8 give the parameters.

void ScreenDevice::ExecInterrupt(VERControler * lr) {
  linkreg=lr;
  if (active) {
    //Function 1 : clear the screen
    //
    if (linkreg->GetRegister(2)->GetLow()==1) {
      printf("%c","");
	  fflush(stdout);
    }
    //Function 2 : print a char at the current position
    //
    // reg3lo : the character to print
    if (linkreg->GetRegister(2)->GetLow()==2) {
      unsigned char c = (unsigned char)linkreg->GetRegister(3)->GetLow();
      printf("%c",c);
	  fflush(stdout);
    }

    //Function 3 : move the character pointer
    //
    // reg3hi : x
    // reg3lo : y
    if (linkreg->GetRegister(2)->GetLow()==3) {
      unsigned char c = (unsigned char)linkreg->GetRegister(3)->GetLow();
      printf("%c",c);
	  fflush(stdout);
    }
    //Function 4 :
    //
    // reg 3hi : x
    if (linkreg->GetRegister(2)->GetLow()==4) {
      unsigned char c = (unsigned char)linkreg->GetRegister(3)->GetLow();
    }
	//Function 5 :
    //
    // reg 3hi : x
    if (linkreg->GetRegister(2)->GetLow()==5) {
      unsigned char c = (unsigned char)linkreg->GetRegister(3)->GetLow();
    }
    //Function 6 :
    //
    // reg 3hi : x
    if (linkreg->GetRegister(2)->GetLow()==6) {
      unsigned char c = (unsigned char)linkreg->GetRegister(3)->GetLow();
    }
  }
}


void ScreenDevice::LocalShutdown() {
}

void ScreenDevice::LocalStart() {
}

void ScreenDevice::LocalStop() {
}

ScreenDevice::~ScreenDevice() {
}

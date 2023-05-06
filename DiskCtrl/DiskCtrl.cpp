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

#include "DiskCtrl/DiskCtrl.h"

DiskCtrl::DiskCtrl(char * fpath,char * fname,int nbcylindre,int nbpiste, int nbbloc) {
	VirtualDiskName= new char[strlen(fname)];
	VirtualDiskPath= new char[strlen(fpath)];
	DiskCylinder = nbcylindre;
	DiskPiste = nbpiste;
	DiskBloc = nbbloc;
}

void DiskCtrl::LocalInit() {
  strcpy(descriptor,"Disk Controler (c) O.Moulin");
  deviceID = 2;
  maxinterrupt = 1;
  interrupt = new unsigned char[1];
  interrupt[0]=13;
}

// The Text Screen Driver is linked on the 13 interrupt
//
// The register 2 give the choice beetween the functions called for this device
// The register 3,4,5,6,7,8 give the parameters.

void DiskCtrl::ExecInterrupt(VERControler * lr) {
  linkreg=lr;
  if (active) {
    //Function 1 : ecrit un mot � l'emplacement x piste et y cylindre , z decalage;
    //
    // register 3hi : piste
    // register 3lo : cylindre
    // register 4hi : bloc
    // register 5 : data
    if (linkreg->GetRegister(2)->GetLow()==1) {
      rewind(VirtualFile);
	  unsigned short int piste = (unsigned char)linkreg->GetRegister(3)->GetHigh();
      unsigned short int cylindre = (unsigned char)linkreg->GetRegister(3)->GetLow();
      unsigned short int bloc = (unsigned char)linkreg->GetRegister(4)->GetHigh();
      unsigned short int data = (unsigned char)linkreg->GetRegister(5)->GetValue();
	  fseek(VirtualFile,piste*DiskBloc*cylindre+bloc,0);
	  fprintf(VirtualFile,"%ud",data);
    }
    //Function 2 : lit un mot � l'enplacement x piste et y cylindre , z decalage;
    //
    // reg3hi : piste
	// reg3lo : cylindre;
	// reg4hi : bloc
	// register 5 : data
    if (linkreg->GetRegister(2)->GetLow()==2) {
	  rewind(VirtualFile);
	  fseek(VirtualFile,piste*DiskBloc*cylindre+bloc,0);
	  fscanf(VirtualFile,"%ud",&date);
    }
    //Function 3 : move the character pointer
    //
    // reg3hi : x
    // reg3lo : y
    if (linkreg->GetRegister(2)->GetLow()==3) {
      unsigned char c = (unsigned char)linkreg->GetRegister(3)->GetLow();
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


void DiskCtrl::LocalShutdown() {
}

void DiskCtrl::LocalStart() {
  char filename[255];
  sprintf(filename,"%s\/%s",VirtualFilePath,VirtualFileName);
  VirtualFile = fopen(filename,"rw");
}

void DiskCtrl::LocalStop() {
	fclose(VirtualFile);
}

DiskCtrl::~DiskCtrl() {
}

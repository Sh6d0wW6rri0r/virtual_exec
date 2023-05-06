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

#ifndef VIRTUALEXEDEF
#define VIRTUALEXEDEF

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Commun/VECommun.h"




// -------------------------------------------------------------------------
// Classe MEMSPACE : classe generique de definition d un emplacement memoire
// -------------------------------------------------------------------------
VECLASS VEMemSpace {
  protected:
  union {
    struct {
	  unsigned short int low;
          unsigned short int high;
    } bit16;
	struct {
	  unsigned char lowL;
          unsigned char lowH;
	  unsigned char highL;
	  unsigned char highH;
	} bit8;
    unsigned int bit32;
  } val;

  public:
  VEMemSpace();
  void SetValue(unsigned int b);
  unsigned int GetValue();
  void SetLow(unsigned short l);
  void SetHigh(unsigned short h);
  unsigned short int GetLow();
  unsigned short int GetHigh();
  virtual ~VEMemSpace();
};

// ------------------------------------------------------------
// Classe REGISTER : classe de definition des registres memoire
// ------------------------------------------------------------

// Les registres 0 et 1 sont r�serv� pour g�rer les pointeurs memoire
VECLASS VERControler {
  public :
  virtual VEMemSpace * GetRegister(unsigned char nreg);
  virtual void SetRegister (unsigned char nreg,VEMemSpace * val);
  virtual void SetRegister (unsigned char nreg,unsigned int val);
  virtual void Shutdown();
};

VECLASS VERegister : public VERControler {
  private:
  unsigned int reg[255];

  public:
  VERegister();
  void Init();
  VEMemSpace * GetRegister (unsigned char nreg);
  void SetRegister (unsigned char nreg,VEMemSpace * val);
  void SetRegister (unsigned char nreg,unsigned int val);
  void Shutdown();
  virtual ~VERegister();
};



// --------------------------------------------------
// Classe MEMORY : classe de definition de la memoire
// --------------------------------------------------

// Un lien est fait avec la classe registre pour gerer les pointeurs
// en memoire
// les registres 0 et 1 sont respectivement :
//     0 : le registre du pointeur memoire
//     1 : le registre du pointeur programme


// ----------------------------------------------
// Classe STACK : classe de definition de la pile
// ----------------------------------------------

VECLASS VEStack {
  private:
  unsigned int curptr;
  unsigned int stackmax;
  unsigned int * space;

  public:
  VEStack();
  void Init(unsigned int smax);
  VEMemSpace * Pop();
  void Push(VEMemSpace * val);
  void Shutdown();
  virtual ~VEStack();
};

// ------------------------------------------------------------------
// Classe INTERRUPT : classe de definition des tables d interruptions
// ------------------------------------------------------------------
VECLASS VEInterrupt {
  private:
  unsigned char interrupttable[255];

  public:
  VEInterrupt();
  void Init();
  unsigned char GetDevice(unsigned char interrupt);
  void SetDevice(unsigned char interrupt,unsigned char devID);
  void DelDevice(unsigned char devID);
  virtual ~VEInterrupt();
};

VECLASS VEFlag {
  private:
  bool equalzero;
  bool divbyzero;
  bool equal;
  bool inf;
  bool sup;

  public:
  VEFlag();
  void Init();
  bool IsFlagEqualZero();
  bool IsFlagDivByZero();
  bool IsFlagEqual();
  bool IsFlagSup();
  bool IsFlagInf();
  void SetFlagEqualZero(bool state);
  void SetFlagDivByZero(bool state);
  void SetFlagEqual(bool state);
  void SetFlagSup(bool state);
  void SetFlagInf(bool state);
  virtual ~VEFlag();
};

#endif

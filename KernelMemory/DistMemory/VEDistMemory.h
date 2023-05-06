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

#ifndef DISTMEMORY
#define DISTMEMORY

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "KernelDef/KernelDef.h"
#include "KernelMemory/Memory/VEMemory.h"
#include "Commun/VECommun.h"

VECLASS VEDistMemory : public VEMemory {
  private:
  NetworkClient * nc;

  public:
  VEDistMemory();
  void SetServer(char * serv,int prt);
  void Init();
  unsigned int GetMemoryMax();
  VEMemSpace * GetAddress(unsigned int ma);
  void StoreAddress(unsigned int ma,VEMemSpace * val);
  void LocalShutdown();
  virtual ~VEDistMemory();
};

#endif

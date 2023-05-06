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

#include "KernelMemory/MemoryCard/VEMemoryCard.h"

VEMemoryCard::VEMemoryCard() {
}

void VEMemoryCard::Init(unsigned int mmax) {
 unsigned int i;
 space = new unsigned int[mmax];
 for (i = 0;i < mmax; i++) {
   space[i] = 0;
 }
 memmax = mmax;
}

unsigned int VEMemoryCard::GetMemoryMax() {
  return memmax;
}

VEMemSpace * VEMemoryCard::GetAddress(unsigned int ma) {
#ifdef DEBUG
  printf("\nMEMCARD >> Getting the address %d value",ma);
#endif
  VEMemSpace * tmp = new VEMemSpace();
  tmp->SetValue(space[ma]);
  return tmp;
}


void VEMemoryCard::StoreAddress(unsigned int ma,VEMemSpace * val) {
#ifdef DEBUG
    printf("\nMEMCARD >> Storing the value at %dl",ma);
#endif
  space[ma] = val->GetValue();
}

void VEMemoryCard::LocalShutdown() {
}

VEMemoryCard::~VEMemoryCard() {
  delete space;
}

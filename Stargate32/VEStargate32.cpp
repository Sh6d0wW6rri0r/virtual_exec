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

#include "Stargate32/VEStargate32.h"


Stargate32::Stargate32() {
    OPCODES = new char* [255];
    OPCODES[0] = "NOP";
    OPCODES[1] = "MOVRM";
    OPCODES[2] = "MOVMR";
    OPCODES[3] = "MOVVR";
    OPCODES[4] = "MOVRR";
    OPCODES[5] = "ADDVR";
    OPCODES[6] = "ADDRR";
    OPCODES[7] = "SUBVR";
    OPCODES[8] = "SUBRR";
    OPCODES[9] = "MULVR";
    OPCODES[10] = "MULRR";
    OPCODES[11] = "DIVVR";
    OPCODES[12] = "DIVRR";
    OPCODES[13] = "JMP";
    OPCODES[14] = "JMPZ";
    OPCODES[15] = "JMPNZ";
    OPCODES[16] = "JMPE";
    OPCODES[17] = "JMPNE";
    OPCODES[18] = "JMPG";
    OPCODES[19] = "JMPS";
    OPCODES[20] = "JMPDZ";
    OPCODES[21] = "JMPNDZ";
    OPCODES[22] = "CMPVR";
    OPCODES[23] = "CMPRM";
    OPCODES[24] = "CMPRR";
    OPCODES[25] = "PUSHR";
    OPCODES[26] = "POPR";
    OPCODES[27] = "PUSHV";
    OPCODES[28] = "PUSHM";
    OPCODES[29] = "POPM";
    OPCODES[30] = "INTR";
    OPCODES[31] = "SUB";
    OPCODES[32] = "RET";
    OPCODES[33] = "JPC";
    for (int i = 34;i < 254;i++) {
        OPCODES[i] = "NOP";
    }
    OPCODES[254] = "STP";
}

void Stargate32::LocalInit() {
  strcpy(descriptor,"SG-01 : StarGate Processor v1.0 (c) O.Moulin");
  id=1;
}


void Stargate32::Execute(unsigned int add) {
  unsigned short currentop=0;
  // Opcode 254 = HardWare STOP
  connectedregister->SetRegister(0,add);
  while (currentop!=254) {
    VEMemSpace * opc = GetProgram();
    currentop=opc->GetLow();
//#ifdef DEBUG
      printf("\nPROCESSOR >> OP : %u EXT : %u PRGPOS : %u",currentop,opc->GetHigh(), GetCurrentProgram()   );
//#endif
    // Opcode 1  = Move From Register to Memory
    if (currentop==1) {
      MoveNextProgram();
      VEMemSpace * extd = GetProgram();
      MoveNextProgram();
      connectedflag->SetFlagEqualZero((connectedregister->GetRegister((unsigned char)opc->GetHigh())->GetValue()==0));
      linkedmemory->StoreAddress(extd->GetValue(),connectedregister->GetRegister((unsigned char)opc->GetHigh()));
    }
    // Opcode 2  = Move From Memory to Register
    if (currentop==2) {
      MoveNextProgram();
      VEMemSpace * extd = GetProgram();
      MoveNextProgram();
      connectedflag->SetFlagEqualZero((linkedmemory->GetAddress(extd->GetValue())->GetValue()==0));
      connectedregister->SetRegister((unsigned char)opc->GetHigh(),linkedmemory->GetAddress(extd->GetValue()));
    }
    // Opcode 3  = Move Value to Register
    if (currentop==3) {
      MoveNextProgram();
      VEMemSpace * extd = GetProgram();
      MoveNextProgram();
      connectedflag->SetFlagEqualZero((opc->GetValue()==0));
#ifdef DEBUG
        printf("MVTR : %d %d",(unsigned char)opc->GetHigh(),extd->GetValue());
#endif
	  connectedregister->SetRegister((unsigned char)opc->GetHigh(),extd->GetValue());
    }
    // Opcode 4  = Move Register to Register
    if (currentop==4) {
      MoveNextProgram();
      VEMemSpace * extd = GetProgram();
      MoveNextProgram();
      connectedflag->SetFlagEqualZero((connectedregister->GetRegister((unsigned char)extd->GetLow())->GetValue()==0));
      unsigned int valeur = connectedregister->GetRegister((unsigned char)extd->GetLow())->GetValue();
      connectedregister->SetRegister((unsigned char)opc->GetHigh(),valeur);
    }
    // Opcode 5  = Add Value to Register
    if (currentop==5) {
      MoveNextProgram();
      VEMemSpace * extd = GetProgram();
      MoveNextProgram();
      unsigned int valeur = (connectedregister->GetRegister((unsigned char)opc->GetHigh())->GetValue())+(extd->GetValue());
      connectedflag->SetFlagEqualZero((valeur==0));
      connectedregister->SetRegister((unsigned char)opc->GetHigh(),valeur);
    }
    // Opcode 6  = Add Register to Register
    if (currentop==6) {
      MoveNextProgram();
      VEMemSpace * extd = GetProgram();
      MoveNextProgram();
      unsigned int valeur = (connectedregister->GetRegister((unsigned char)opc->GetHigh())->GetValue())+(connectedregister->GetRegister((unsigned char)extd->GetLow())->GetValue());
      connectedflag->SetFlagEqualZero((valeur==0));
      connectedregister->SetRegister((unsigned char)opc->GetHigh(),valeur);
    }
    // Opcode 7  = Sub Value to Register
    if (currentop==7) {
      unsigned int valeur=0;
      MoveNextProgram();
      VEMemSpace * extd = GetProgram();
      MoveNextProgram();
      if (connectedregister->GetRegister((unsigned char)opc->GetHigh())->GetValue()>extd->GetValue()) {
        valeur = (connectedregister->GetRegister((unsigned char)opc->GetHigh())->GetValue())-(extd->GetValue());
      }
      else {
        valeur = 0;
      }
      connectedflag->SetFlagEqualZero((valeur==0));
      connectedregister->SetRegister((unsigned char)opc->GetHigh(),valeur);
    }
    // Opcode 8  = Sub Register to Register
    if (currentop==8) {
      unsigned int valeur = 0;
      MoveNextProgram();
      VEMemSpace * extd = GetProgram();
      MoveNextProgram();
      if (connectedregister->GetRegister((unsigned char)opc->GetHigh())->GetValue()>connectedregister->GetRegister((unsigned char)extd->GetLow())->GetValue()) {
        valeur = (connectedregister->GetRegister((unsigned char)opc->GetHigh())->GetValue())-(connectedregister->GetRegister((unsigned char)extd->GetLow())->GetValue());
      }
      else {
        valeur =0;
      }
      connectedflag->SetFlagEqualZero((valeur==0));
      connectedregister->SetRegister((unsigned char)opc->GetHigh(),valeur);
    }
    // Opcode 9  = Mult Register by Value
    if (currentop==9) {
      MoveNextProgram();
      VEMemSpace * extd = GetProgram();
      MoveNextProgram();
      unsigned int valeur = (connectedregister->GetRegister((unsigned char)opc->GetHigh())->GetValue())*(extd->GetValue());
      connectedflag->SetFlagEqualZero((valeur==0));
      connectedregister->SetRegister((unsigned char)opc->GetHigh(),valeur);
    }
    // Opcode 10 = Mult Register by Register
    if (currentop==10) {
      MoveNextProgram();
      VEMemSpace * extd = GetProgram();
      MoveNextProgram();
      unsigned int valeur = (connectedregister->GetRegister((unsigned char)opc->GetHigh())->GetValue())*(connectedregister->GetRegister((unsigned char)extd->GetLow())->GetValue());
      connectedflag->SetFlagEqualZero((valeur==0));
      connectedregister->SetRegister((unsigned char)opc->GetHigh(),valeur);
    }
    // Opcode 11 = Div Register by Value
    if (currentop==11) {
      unsigned int valeur=0;
      MoveNextProgram();
      VEMemSpace * extd = GetProgram();
      MoveNextProgram();
      if (opc->GetValue()>0) {
        valeur = connectedregister->GetRegister((unsigned char)opc->GetHigh())->GetValue()/extd->GetValue();
      }
      else {
        valeur=0;
      }
      connectedflag->SetFlagDivByZero((extd->GetValue()==0));
      connectedflag->SetFlagEqualZero((valeur==0));
      connectedregister->SetRegister((unsigned char)opc->GetHigh(),valeur);
    }
    // Opcode 12 = Div Register by Register
    if (currentop==12) {
      unsigned int valeur = 0;
      MoveNextProgram();
      VEMemSpace * extd = GetProgram();
      MoveNextProgram();
      if (connectedregister->GetRegister((unsigned char)extd->GetLow())->GetValue()>0) {
        valeur = connectedregister->GetRegister((unsigned char)opc->GetHigh())->GetValue()/connectedregister->GetRegister((unsigned char)extd->GetLow())->GetValue();
      }
      else {
        valeur=0;
      }
      connectedflag->SetFlagDivByZero((connectedregister->GetRegister((unsigned char)extd->GetLow())->GetValue()==0));
      connectedflag->SetFlagEqualZero((valeur==0));
      connectedregister->SetRegister((unsigned char)opc->GetHigh(),valeur);
    }
    // Opcode 13 = JUMP to Memory
    if (currentop==13) {
      MoveNextProgram();
      VEMemSpace * extd = GetProgram();
      MoveNextProgram();
      MoveToProgram(extd->GetValue());
    }
    // Opcode 14 = JUMP to Memory if EqualZero
    if (currentop==14) {
      MoveNextProgram();
      VEMemSpace * extd = GetProgram();
      MoveNextProgram();
      if (connectedflag->IsFlagEqualZero()) {
        MoveToProgram(extd->GetValue());
      }
    }
    // Opcode 15 = JUMP to Memory if not EqualZero
    if (currentop==15) {
      MoveNextProgram();
      VEMemSpace * extd = GetProgram();
      MoveNextProgram();
      if (!connectedflag->IsFlagEqualZero()) {
        MoveToProgram(extd->GetValue());
      }
    }
    // Opcode 16 = JUMP to Memory if Equal
    if (currentop==16) {
      MoveNextProgram();
      VEMemSpace * extd = GetProgram();
      MoveNextProgram();
      if (connectedflag->IsFlagEqual()) {
        MoveToProgram(extd->GetValue());
      }
    }
    // Opcode 17 = JUMP to Memory if not Equal
    if (currentop==17) {
      MoveNextProgram();
      VEMemSpace * extd = GetProgram();
      MoveNextProgram();
      if (!connectedflag->IsFlagEqual()) {
        MoveToProgram(extd->GetValue());
      }
    }

    // Opcode 18 = JUMP to Memory if Sup
    if (currentop==18) {
      MoveNextProgram();
      VEMemSpace * extd = GetProgram();
      MoveNextProgram();
      if (connectedflag->IsFlagSup()) {
        MoveToProgram(extd->GetValue());
      }
    }
    // Opcode 19 = JUMP to Memory if Inf
    if (currentop==19) {
      MoveNextProgram();
      VEMemSpace * extd = GetProgram();
      MoveNextProgram();
      if (connectedflag->IsFlagInf()) {
        MoveToProgram(extd->GetValue());
      }
    }
    // Opcode 20 = JUMP to Memory if DivByZero
    if (currentop==20) {
      MoveNextProgram();
      VEMemSpace * extd = GetProgram();
      MoveNextProgram();
      if (connectedflag->IsFlagDivByZero()) {
        MoveToProgram(extd->GetValue());
      }
    }
    // Opcode 21 = JUMP to Memory if not DivByZero
    if (currentop==21) {
      MoveNextProgram();
      VEMemSpace * extd = GetProgram();
      MoveNextProgram();
      if (!connectedflag->IsFlagDivByZero()) {
        MoveToProgram(extd->GetValue());
      }
    }
    // Opcode 22 = CMP Register to Value
    if (currentop==22) {
      MoveNextProgram();
      VEMemSpace * extd = GetProgram();
      MoveNextProgram();
      if (connectedregister->GetRegister((unsigned char)opc->GetHigh())->GetValue() == extd->GetValue()) {
        connectedflag->SetFlagSup(false);
        connectedflag->SetFlagInf(false);
        connectedflag->SetFlagEqual(true);
      }
      if (connectedregister->GetRegister((unsigned char)opc->GetHigh())->GetValue() > extd->GetValue()) {
        connectedflag->SetFlagSup(true);
        connectedflag->SetFlagInf(false);
        connectedflag->SetFlagEqual(false);
      }
      if (connectedregister->GetRegister((unsigned char)opc->GetHigh())->GetValue() < extd->GetValue()) {
        connectedflag->SetFlagSup(false);
        connectedflag->SetFlagInf(true);
        connectedflag->SetFlagEqual(false);
      }
      if (connectedregister->GetRegister((unsigned char)opc->GetHigh())->GetValue() == 0) {
        connectedflag->SetFlagEqualZero(true);
      }
    }
    // Opcode 23 = CMP Register to Memory
    if (currentop==24) {
      MoveNextProgram();
      VEMemSpace * extd = GetProgram();
      MoveNextProgram();
      unsigned int valeur = linkedmemory->GetAddress(extd->GetValue())->GetValue();
      if (connectedregister->GetRegister((unsigned char)opc->GetHigh())->GetValue() == valeur) {
        connectedflag->SetFlagSup(false);
        connectedflag->SetFlagInf(false);
        connectedflag->SetFlagEqual(true);
      }
      if (connectedregister->GetRegister((unsigned char)opc->GetHigh())->GetValue() > valeur) {
        connectedflag->SetFlagSup(true);
        connectedflag->SetFlagInf(false);
        connectedflag->SetFlagEqual(false);
      }
      if (connectedregister->GetRegister((unsigned char)opc->GetHigh())->GetValue() < valeur) {
        connectedflag->SetFlagSup(false);
        connectedflag->SetFlagInf(true);
        connectedflag->SetFlagEqual(false);
      }
      if (connectedregister->GetRegister((unsigned char)opc->GetHigh())->GetValue() == 0) {
        connectedflag->SetFlagEqualZero(true);
      }
    }
    // Opcode 24 = CMP Register to Register
    if (currentop==24) {
      MoveNextProgram();
      VEMemSpace * extd = GetProgram();
      MoveNextProgram();
      unsigned int valeur = connectedregister->GetRegister((unsigned char)extd->GetLow())->GetValue();
      if (connectedregister->GetRegister((unsigned char)opc->GetHigh())->GetValue() == valeur) {
        connectedflag->SetFlagSup(false);
        connectedflag->SetFlagInf(false);
        connectedflag->SetFlagEqual(true);
      }
      if (connectedregister->GetRegister((unsigned char)opc->GetHigh())->GetValue() > valeur) {
        connectedflag->SetFlagSup(true);
        connectedflag->SetFlagInf(false);
        connectedflag->SetFlagEqual(false);
      }
      if (connectedregister->GetRegister((unsigned char)opc->GetHigh())->GetValue() < valeur) {
        connectedflag->SetFlagSup(false);
        connectedflag->SetFlagInf(true);
        connectedflag->SetFlagEqual(false);
      }
      if (connectedregister->GetRegister((unsigned char)opc->GetHigh())->GetValue() == 0) {
        connectedflag->SetFlagEqualZero(true);
      }
    }
    // Opcode 25 = PUSH Register
    if (currentop==25) {
      MoveNextProgram();
      MoveNextProgram();
      linkedstack->Push(connectedregister->GetRegister((unsigned char)opc->GetLow()));
    }
    // Opcode 26 = POP Register
    if (currentop==26) {
      MoveNextProgram();
      MoveNextProgram();
      connectedregister->SetRegister((unsigned char)opc->GetLow(),linkedstack->Pop());
    }
    // Opcode 27 = PUSH Value
    if (currentop==27) {
      MoveNextProgram();
      VEMemSpace * extd = GetProgram();
      MoveNextProgram();
      linkedstack->Push(extd);
    }
    // Opcode 28 = PUSH Memory
    if (currentop==28) {
      MoveNextProgram();
      VEMemSpace * extd = GetProgram();
      MoveNextProgram();
      linkedstack->Push(linkedmemory->GetAddress(extd->GetValue()));
    }
    // Opcode 29 = POP Memory
    if (currentop==29) {
      MoveNextProgram();
      VEMemSpace * extd = GetProgram();
      MoveNextProgram();
      linkedmemory->StoreAddress(extd->GetValue(),linkedstack->Pop());
    }
    // Opcode 30 = Interrupt Call
    if (currentop==30) {
      unsigned char mydev=linkedinterrupt->GetDevice((unsigned char)opc->GetHigh());
      MoveNextProgram();
      MoveNextProgram();
      VEDevice * d = linkeddevice->FindDevice(mydev);
      if (d!=NULL) {
        d->ExecInterrupt(connectedregister);
      }
      else {
        printf("\nError : device ID:%d %s",mydev," not connected");
      }
    }
    // Opcode 31 = SUB : Go to Sub-program
    if (currentop==31) {
      MoveNextProgram();
      VEMemSpace * extd = GetProgram();
      MoveNextProgram();
      linkedstack->Push(connectedregister->GetRegister(0));
      MoveToProgram(extd->GetValue());
    }
    // Opcode 32 = RET : Return from Sub-program
    if (currentop==32) {
      MoveNextProgram();
      MoveNextProgram();
      connectedregister->SetRegister(0,linkedstack->Pop());
    }
	// Opcode 33 = JPC : Jump with next proc
    if (currentop==33) {
      MoveNextProgram();
      VEMemSpace * extd = GetProgram();
      MoveNextProgram();
      StartProgramOnNextProcessor(extd->GetValue());
    }
  }
}

void Stargate32::LocalShutdown() {
}

Stargate32::~Stargate32() {
}

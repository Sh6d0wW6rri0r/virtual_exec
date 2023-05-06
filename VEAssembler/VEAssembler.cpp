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

// VEAssembler.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include "KernelDef/KernelDef.h"
#include "Stargate32/VEStargate32.h"

int main(int argv,char* argc[])
{
    std::cout << "Starting Starg:ate 32 compiler ... (c) O.Moulin";
    fflush(stdout);
    std::ifstream finput("test.asm");
    std::ofstream foutput("test.prg", std::ios::binary);
    std::string op = "NOP";
    std::string par1 = "0";
    std::string par2 = "0";
    int lcpt = 0;
    Stargate32* proc;
    proc = new Stargate32();
    proc->LocalInit();
    std::cout << "\nCompiling input file : " << "test.asm" << std::flush;
    std::cout << "\nGenerating output file : " << "test.prg" << std::flush;
    fflush(stdout);
    std::string lineread;
    while (finput >> op >> par1 >> par2) {
        lcpt++;
        std::cout << "\nCMD : " << lcpt << " - "  << op << " " << par1 << " " << par2 << std::flush;
        VEMemSpace* m = new VEMemSpace();
        unsigned int  i = proc->getOpcode(op.c_str());
        std::cout << "\nOPCODE : " << i << " " << par1.c_str() << " " << par2.c_str();
        if (i!=255) {
            m->SetLow(i);
            m->SetHigh(atoi(par1.c_str()));
            std::cout << m->GetValue() << "";
            unsigned int m1 = m->GetValue();
            foutput.write((char*)&m1, sizeof(unsigned int));
            m->SetValue(atol(par2.c_str()));
            std::cout << m->GetValue();
            unsigned int m2 = m->GetValue();
            foutput.write((char*)&m2, sizeof(unsigned int));
        }
        else {
            std::cout << "Parse error line " << lcpt << " - " << op << " " << par1 << " " << par2  << std::flush;
        }
        delete m;
    }
    finput.close();
    foutput.close();
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

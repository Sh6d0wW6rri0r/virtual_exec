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

// VEConsole.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "KernelEngine/KernelEngine.h"
#include "KernelDevice/KernelDevice.h"
#include "KernelMemory/KernelMemory.h"
#include "Stargate32/VEStargate32.h"
#include "ScreenDevice/ScreenDevice.h"
#include "GraphicDevice/GraphicDevice.h"
#include <stdlib.h>
#include <iostream>


int main(int argv,char ** argc)
{
	VEEngine* e = new VEEngine();
	Stargate32* st = new Stargate32();
	//e->ConnectSupervisor("127.0.0.1",9999);
	e->Init(255, st);
	Stargate32* st2 = new Stargate32();
	e->AddProcessor(st2);
	Stargate32* st3 = new Stargate32();
	e->AddProcessor(st3);
	VEMemoryCard* m1 = new VEMemoryCard();
	m1->Init(64000);
	e->AddMemoryCard(m1);
	ScreenDevice* d = new ScreenDevice();
	e->AddDevice((VEDevice*)d);
	GraphicDevice* g = new GraphicDevice();
	e->AddDevice((VEDevice*)g);
	d->Start();
	e->LoadFile(argc[1]);
	e->StartEngine();
	e->StopEngine();
	while(1);
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

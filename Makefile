ROOT_COMP = `pwd`
GCCOPT = -c -DDEBUG -g
X11LIB = /usr/X11R6/lib

all: VirtualExec

clean:
	echo "--------------------------------------------------------------------------------"
	echo "Cleaning the binary files ..."
	echo ""
	rm lib/*
	rm bin/VEAsm
	rm bin/VEConsole

install:
	sudo cp lib/* /usr/local/lib/

VirtualExec: Commun Def MemoryControler RemoteMemory Devices Processor Engine RemoteDevice Assembler ExampleDevices Console ExampleRemoteDevices

Def: lib/libVEDef.dylib
	echo "--------------------------------------------------------------------------------"
	echo "Building the Generic definition file ..."
	echo ""

lib/libVEDef.dylib: ./KernelDef/Def/VEDef.cpp
	clang++ -I./ -dynamiclib -o lib/libVEDef.dylib ./KernelDef/Def/VEDef.cpp

Commun: lib/libVECommun.dylib
	echo "--------------------------------------------------------------------------------"
	echo "Building the Common Utilities file ..."
	echo ""

lib/libVECommun.dylib: Commun/Network/VEReadSend.cpp
	clang++ -I./ -dynamiclib -o lib/libVECommun.dylib Commun/Network/VEReadSend.cpp

MemoryControler: lib/libVEMemoryControler.dylib
	echo "--------------------------------------------------------------------------------"
	echo "Building the Memory Controler file ..."
	echo ""

lib/libVEMemoryControler.dylib: KernelMemory/MemoryControler/VEMemCtrl.cpp KernelMemory/MemoryCard/VEMemoryCard.cpp KernelMemory/DistMemory/VEDistMemory.cpp KernelMemory/Memory/VEMemory.cpp KernelMemory/MControler/VEMControler.cpp
	clang++ -L./lib -lVEDef -lVECommun -dynamiclib -o lib/libVEMemoryControler.dylib -I./ KernelMemory/MemoryControler/VEMemCtrl.cpp KernelMemory/MemoryCard/VEMemoryCard.cpp KernelMemory/DistMemory/VEDistMemory.cpp KernelMemory/Memory/VEMemory.cpp KernelMemory/MControler/VEMControler.cpp

RemoteMemory: lib/libVERemoteMemory.dylib
	echo "--------------------------------------------------------------------------------"
	echo "Building the Remote Memory file ..."
	echo ""

lib/libVERemoteMemory.dylib: KernelMemory/RemoteCard/VERemoteCard.cpp
	clang++ -L./lib -lVEDef -lVECommun -dynamiclib -o lib/libVERemoteMemory.dylib -I./ KernelMemory/RemoteCard/VERemoteCard.cpp

Devices: lib/libVEDevices.dylib
	echo "--------------------------------------------------------------------------------"
	echo "Building the Device controler file ..."
	echo ""

lib/libVEDevices.dylib: KernelDevice/LocalDevice/VEDevice.cpp KernelDevice/DistDevice/VEDistDevice.cpp KernelDevice/DevCtrl/VEDevCtrl.cpp
	clang++ -L./lib -lVEDef -lVECommun -dynamiclib -o lib/libVEDevices.dylib -I./ KernelDevice/LocalDevice/VEDevice.cpp KernelDevice/DistDevice/VEDistDevice.cpp KernelDevice/DevCtrl/VEDevCtrl.cpp

RemoteDevice: lib/libVERemoteDevice.dylib
	echo "--------------------------------------------------------------------------------"
	echo "Building the Generic Remote Device Server file ..."
	echo ""

lib/libVERemoteDevice.dylib: KernelDevice/RemoteDevice/VERemDevice.cpp KernelDevice/RemoteDevice/RemRControler/VERemRControler.cpp KernelDevice/RemoteDevice/RemMControler/VERemMControler.cpp
	clang++ -L./lib -lVECommun -lVEDef -lVEDevices -lVEMemoryControler -dynamiclib -o lib/libVERemoteDevice.dylib -I./ KernelDevice/RemoteDevice/VERemDevice.cpp KernelDevice/RemoteDevice/RemRControler/VERemRControler.cpp KernelDevice/RemoteDevice/RemMControler/VERemMControler.cpp

Processor: lib/libVEProcessor.dylib
	echo "--------------------------------------------------------------------------------"
	echo "Building the Generic Processor file ..."
	echo ""

lib/libVEProcessor.dylib: KernelProcessor/Processor/VEProcessor.cpp Stargate32/VEStargate32.cpp KernelProcessor/ProcControler/VEProcCtrl.cpp
	clang++ -L./lib -lVEDef -lVEDevices -lVEMemoryControler -dynamiclib -o lib/libVEProcessor.dylib -I./ KernelProcessor/Processor/VEProcessor.cpp Stargate32/VEStargate32.cpp KernelProcessor/ProcControler/VEProcCtrl.cpp

Engine: lib/libVEEngine.dylib
	echo "--------------------------------------------------------------------------------"
	echo "Building the Kernel Engine file ..."
	echo ""

lib/libVEEngine.dylib: KernelEngine/Engine/VEEngine.cpp KernelEngine/StateServer/VEStateServer.cpp
	clang++ -L./lib -lpthread -lVEDef -lVECommun -lVEDevices -lVEMemoryControler -lVEProcessor -dynamiclib -o lib/libVEEngine.dylib -I./ KernelEngine/Engine/VEEngine.cpp KernelEngine/StateServer/VEStateServer.cpp


ExampleDevices: lib/libVEExampleDevices.dylib
	echo "--------------------------------------------------------------------------------"
	echo "Building the Example Devices Package file ..."
	echo ""

lib/libVEExampleDevices.dylib: ScreenDevice/ScreenDevice.cpp GraphicDevice/GraphicDevice.cpp
	clang++ -L./lib -lVEDef -lVEDevices -dynamiclib -o lib/libVEExampleDevices.dylib -I./ ScreenDevice/ScreenDevice.cpp GraphicDevice/GraphicDevice.cpp -framework OpenGL -framework GLUT

ExampleRemoteDevices: bin/ScreenTextServer bin/GraphicServer
	echo "--------------------------------------------------------------------------------"
	echo "Building the Example Remote Server Devices file ..."
	echo ""

bin/ScreenTextServer: ScreenTextServer/ScreenTextServer.cpp
	clang++ -L./lib -I./ -lVECommun -lVEDevices -lVERemoteDevice -lVEExampleDevices -o bin/ScreenTextServer ScreenTextServer/ScreenTextServer.cpp

bin/GraphicServer: GraphicServer/GraphicServer.cpp
	clang++ -L./lib -I./ -lVECommun -lVEDevices -lVERemoteDevice -lVEExampleDevices -o bin/GraphicServer GraphicServer/GraphicServer.cpp

bin/MemoryCardServer: MemoryCardServer/VEMemoryCardServer.cpp
	clang++ -L./lib -I./ -lVECommun -lVERemoteMemory -o bin/MemoryCardServer MemoryCardServer/MemoryCardServer.cpp
	g++ -I$(ROOT_COMP) -L$(ROOT_COMP)/lib -lVECommun -lVERemoteMemory Examples/RemoteDevices/MemoryCardServer/VEMemoryCardServer.cpp -o bin/VEMemoryCardServer

Assembler: bin/VEAsm
	echo "--------------------------------------------------------------------------------"
	echo "Building the Assembler binary ..."
	echo ""

bin/VEAsm: VEAssembler/VEAssembler.cpp
	clang++ -L./lib -I./ -lVEDef -lVECommun -lVEEngine -lVEMemoryControler -lVEProcessor -o bin/VEAsm VEAssembler/VEAssembler.cpp

Console: bin/VEConsole
	echo "--------------------------------------------------------------------------------"
	echo "Building a Console Example binary ..."
	echo ""

bin/VEConsole: VEConsole/VEConsole.cpp
	clang++ -L./lib -I./ -lVECommun -lVEEngine -lVEProcessor -lVEMemoryControler -lVEDevices -lVEExampleDevices -o bin/VEConsole VEConsole/VEConsole.cpp


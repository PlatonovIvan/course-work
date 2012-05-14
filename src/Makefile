CXX = g++
DEL_FILE = rm -f

OBJECTS = main.o SimpleFunctions.o Stack.o PolyUnpack.o PE_Reader.o Emulator_LibEmu.o

TARGET		= ../bin/Algorithm

DOXY = doxygen 

all: $(TARGET) doc clean

bin: 
	 -$(DEL_FILE) ../bin/
	 $(TARGET) 

clean:
	-$(DEL_FILE) *.o *~

doc:	
	rm -rf ../doc/	
	mkdir -p ../doc/
	doxygen 2>/dev/null


$(TARGET): main.o SimpleFunctions.o Stack.o PolyUnpack.o PE_Reader.o Emulator_LibEmu.o
	$(CXX) -o $@ main.o  SimpleFunctions.o Stack.o PolyUnpack.o PE_Reader.o Emulator_LibEmu.o libdasm/libdasm.o -lemu -L$(CURDIR)/libemu -Wl,-rpath -Wl,$(CURDIR)/libemu
 
main.o: main.cpp PolyUnpack.h 

	$(CXX) -o $@ -c main.cpp -I$(CURDIR)/libemu/include

SimpleFunctions.o: SimpleFunctions.cpp 
	$(CXX) -o $@ -c SimpleFunctions.cpp

Stack.o: Stack.cpp Stack.h
	$(CXX) -o $@ -c Stack.cpp

PolyUnpack.o: PolyUnpack.cpp PolyUnpack.h InstructionList.h Instruction_Leaf.h Emulator_LibEmu.h PE_Reader.h Stack.h SimpleFunctions.h libdasm/libdasm.h

	$(CXX) -o $@ -c PolyUnpack.cpp

PE_Reader.o: PE_Reader.cpp PE_Reader.h 
	$(CXX) -o $@ -c PE_Reader.cpp

Emulator_LibEmu.o: Emulator_LibEmu.cpp Emulator_LibEmu.h  libemu/include/emu/emu.h libemu/include/emu/emu_cpu.h libemu/include/emu/emu_memory.h
	$(CXX) -o $@ -c Emulator_LibEmu.cpp



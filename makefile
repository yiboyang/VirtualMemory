# makefile for assignment 5
# Yibo Yang, 04/26/2016

EXE = a.out
OBJS = TLB.o PageTable.o BackingStore.o main.o
FLAGS = -g -Wall -std=c++11

$(EXE):	$(OBJS)
	g++ $(FLAGS) -o $(EXE) $(OBJS)
TLB.o:
	g++ $(FLAGS) -c TLB.cpp
PageTable.o:
	g++ $(FLAGS) -c PageTable.cpp
BackingStore.o:
	g++ $(FLAGS) -c BackingStore.cpp
main.o:
	g++ $(FLAGS) -c main.cpp
clean:
	rm -f core $(EXE) $(OBJS)

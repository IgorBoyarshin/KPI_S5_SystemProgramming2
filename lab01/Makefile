all: clean lab1 

# Execution
lab1: lab1.o Table.o TableRow.o
	g++ $(CFLAGS) -o lab1 lab1.o Table.o TableRow.o

lab1.o: lab1.cpp
	g++ $(CFLAGS) -c lab1.cpp

Table.o: Table.cpp Table.h
	g++ $(CFLAGS) -c Table.cpp

TableRow.o: TableRow.cpp TableRow.h
	g++ $(CFLAGS) -c TableRow.cpp

.PHONY: clean

clean:
	rm -f *.o lab1

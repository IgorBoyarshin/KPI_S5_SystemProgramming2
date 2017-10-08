# all: lab2
all: cleanExe lab2

# Execution
lab2: lab2.o Token.o Node.o MooreMachine.o Signal.o
	g++ $(CFLAGS) -o lab2 lab2.o Token.o Node.o MooreMachine.o Signal.o

lab2.o: lab2.cpp state.h Transition.h
	g++ $(CFLAGS) -c lab2.cpp

Token.o: Token.cpp
	g++ $(CFLAGS) -c Token.cpp

MooreMachine.o: MooreMachine.cpp
	g++ $(CFLAGS) -c MooreMachine.cpp

Signal.o: Signal.cpp
	g++ $(CFLAGS) -c Signal.cpp

Node.o: Node.cpp
	g++ $(CFLAGS) -c Node.cpp

clean:
	rm -f *.o lab2

cleanExe:
	rm -f lab2


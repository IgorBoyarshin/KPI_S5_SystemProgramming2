name = lab3
files = $(name).o Token.o TokenName.o LexicalAnalyzer.o

all: cleanExe $(name)

# Execution
$(name): $(files)
	g++ $(CFLAGS) -o $(name) $(files)

# main file
$(name).o: $(name).cpp
	g++ $(CFLAGS) -c $(name).cpp

Token.o: Token.cpp Token.h
	g++ $(CFLAGS) -c Token.cpp

TokenName.o: TokenName.cpp TokenName.h
	g++ $(CFLAGS) -c TokenName.cpp

LexicalAnalyzer.o: LexicalAnalyzer.cpp LexicalAnalyzer.h
	g++ $(CFLAGS) -c LexicalAnalyzer.cpp

clean:
	rm -f *.o $(name)

cleanExe:
	rm -f $(name)


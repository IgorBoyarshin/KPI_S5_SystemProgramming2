mainFileName = lab4
files = Node NodeType Token TokenName SyntaxAnalyzer LexicalAnalyzer
allFiles = $(mainFileName) $(files)
filesObj = $(addsuffix .o, $(allFiles))
filesH = $(addsuffix .h, $(files))

all: cleanExe $(mainFileName)

# Execution
$(mainFileName): $(filesObj)
	g++ $(CFLAGS) $^ -o $@

# Compilation
%.o: %.cpp $(filesH)
	g++ -c $< $(CFLAGS)

# Utils
clean:
	rm -f *.o $(mainFileName)

cleanExe:
	rm -f $(mainFileName)

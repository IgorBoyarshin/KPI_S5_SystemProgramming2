mainFileName = main
classFiles = Argument LexicalAnalyzer Node NodeFunctionNew NodeFunctionEnd NodeStatement Piece PieceType SyntaxAnalyzer Token TokenType Type
justHeaderFiles = NodeType


# translationUnitFiles = $(mainFileName) $(classFiles)
# filesObj = $(addsuffix .o, $(translationUnitFiles))

filesObj = $(addsuffix .o, $(mainFileName) $(classFiles))
filesH = $(addsuffix .h, $(classFiles) $(justHeaderFiles))


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

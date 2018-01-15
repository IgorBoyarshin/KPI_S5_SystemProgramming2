mainFileName = main
classFiles = Argument LexicalAnalyzer Node NodeFunctionNew NodeFunctionEnd NodeStatement Piece PieceType SyntaxAnalyzer Token TokenType Type CodeGenerator
justHeaderFiles = NodeType


filesObj = $(addsuffix .o, $(mainFileName) $(classFiles))
filesH = $(addsuffix .h, $(classFiles) $(justHeaderFiles))


all: cleanExe $(mainFileName)

# Execution
$(mainFileName): $(filesObj)
	g++ $^ -o $@

# Compilation
%.o: %.cpp $(filesH)
	g++ -c $<


# Utils
clean:
	rm -f *.o $(mainFileName)

cleanExe:
	rm -f $(mainFileName)

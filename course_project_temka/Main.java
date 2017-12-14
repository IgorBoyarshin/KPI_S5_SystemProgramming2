import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

import java.util.List;
import java.util.Map;
import java.util.ArrayList;

public class Main {

    public static void main(String[] args) {
        final String input = "int b; bool c; c = 1 + (2 >= (2 / 4 * 2.0));";
        System.out.println("Input: " + input);

        LexicalAnalyzer lexical = new LexicalAnalyzer();
        final List<Token> tokens = lexical.analyze(input);
        if (tokens == null) {
            System.out.println("*** Lexical Analyzer Error ***: " + "Lexical analysis failed.");
            return;
        }

        SyntaxAnalyzer syntax = new SyntaxAnalyzer();
        final StringBuilder code = syntax.parse(tokens);
        if (code == null) {
            System.out.println("*** Syntax Analyzer Error ***: Syntax analysis failed.");
            return;
        }

        OutputGenerator outputGenerator = new OutputGenerator(code, syntax.getRegisteredIds());
        final String output = outputGenerator.generateCode();

        System.out.println("=======================================");
        System.out.println(output);
    }
}

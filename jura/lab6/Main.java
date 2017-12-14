import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

import java.util.List;
import java.util.Map;
import java.util.ArrayList;

public class Main {

    public static void main(String[] args) {
        final String input = "real b; integer^ a; b:=sin(2*a); b:=a;";

        LexicalAnalyzer lexicalAnalyzer = new LexicalAnalyzer();
        final List<Lexeme> lexemes = lexicalAnalyzer.analyze(input);
        if (lexemes == null) {
            System.out.println("Lexical Analyzer Error: " + "Lexical analysis failed.");
            return;
        }

        SyntaxAnalyzer syntaxAnalyzer = new SyntaxAnalyzer();
        final boolean result = syntaxAnalyzer.parse(lexemes);
        if (!result) {
            System.out.println("Syntax Analyzer Error: Syntax analysis failed.");
            return;
        }
    }
}

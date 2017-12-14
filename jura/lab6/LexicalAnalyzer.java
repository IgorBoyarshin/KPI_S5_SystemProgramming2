import java.util.ArrayList;
import java.util.List;

public class LexicalAnalyzer {

    public List<Lexeme> analyze(String input) {
        List<Lexeme> listOfLexemes = new ArrayList<>();
        StringBuilder accumulator = new StringBuilder();
        boolean foundSeparator;

        for (int i = 0; i < input.length(); i++) {
            final char c = input.charAt(i);
            final boolean existsNextChar = (i + 1) < input.length();
            Lexeme lexemeToAdd = null;

            switch (c) {
                case '[':
                    lexemeToAdd = new Lexeme(LexemeType.BRACKETS_OPEN, "[");
                    foundSeparator = true;
                    break;
                case ']':
                    lexemeToAdd = new Lexeme(LexemeType.BRACKETS_CLOSE, "]");
                    foundSeparator = true;
                    break;
                case '(':
                    lexemeToAdd = new Lexeme(LexemeType.PARENTHESES_OPEN, "(");
                    foundSeparator = true;
                    break;
                case ')':
                    lexemeToAdd = new Lexeme(LexemeType.PARENTHESES_CLOSE, ")");
                    foundSeparator = true;
                    break;
                case '{':
                    lexemeToAdd = new Lexeme(LexemeType.BRACES_OPEN, "{");
                    foundSeparator = true;
                    break;
                case '}':
                    lexemeToAdd = new Lexeme(LexemeType.BRACES_CLOSE, "}");
                    foundSeparator = true;
                    break;

                case ';':
                    lexemeToAdd = new Lexeme(LexemeType.SEMICOLON, ";");
                    foundSeparator = true;
                    break;

                case '+':
                    lexemeToAdd = new Lexeme(LexemeType.OPERATOR_PLUS, "+");
                    foundSeparator = true;
                    break;
                case '-':
                    lexemeToAdd = new Lexeme(LexemeType.OPERATOR_MINUS, "-");
                    foundSeparator = true;
                    break;
                case '*':
                    lexemeToAdd = new Lexeme(LexemeType.OPERATOR_MULTIPLY, "*");
                    foundSeparator = true;
                    break;
                case '/':
                    lexemeToAdd = new Lexeme(LexemeType.OPERATOR_DIVIDE, "/");
                    foundSeparator = true;
                    break;

                case '>':
                    if (existsNextChar && (input.charAt(i + 1) == '=')) {
                        lexemeToAdd = new Lexeme(LexemeType.OPERATOR_GREATER_OR_EQUALS, ">=");
                        i++;
                    } else {
                        lexemeToAdd = new Lexeme(LexemeType.OPERATOR_GREATER, ">");
                    }
                    foundSeparator = true;
                    break;
                case '<':
                    if (existsNextChar && (input.charAt(i + 1) == '=')) {
                        lexemeToAdd = new Lexeme(LexemeType.OPERATOR_LESS_OR_EQUALS, "<=");
                        i++;
                    } else if (existsNextChar && (input.charAt(i + 1) == '>')) {
                        lexemeToAdd = new Lexeme(LexemeType.OPERATOR_NOT_EQUALS, "<>");
                        i++;
                    } else {
                        lexemeToAdd = new Lexeme(LexemeType.OPERATOR_LESS, "<");
                    }
                    foundSeparator = true;
                    break;

                case '=':
                    lexemeToAdd = new Lexeme(LexemeType.OPERATOR_ASSIGN, "=");
                    foundSeparator = true;
                    break;

                case '^':
                    lexemeToAdd = new Lexeme(LexemeType.POINTER, "^");
                    foundSeparator = true;
                    break;

                case ',':
                    lexemeToAdd = new Lexeme(LexemeType.COMMA, ",");
                    foundSeparator = true;
                    break;

                case ':':
                    if (existsNextChar && (input.charAt(i + 1) == '=')) {
                        lexemeToAdd = new Lexeme(LexemeType.OPERATOR_ASSIGN, ":=");
                        i++;
                    } else {
                        System.out.println(" Lexical Analyzer Error : " + "Invalid lexeme at " + i + ": " + c + ".");
                        return null;
                    }
                    foundSeparator = true;
                    break;

                case ' ':
                    foundSeparator = true;
                    break;

                default:
                    if (!(isDigit(c) || isLetter(c) || c == '.')) {
                        System.out.println(" Lexical Analyzer Error : " + "Invalid char at " + i + ": " + c + ".");
                        return null;
                    }

                    foundSeparator = false;
                    accumulator.append(c);
            }

            if (foundSeparator || i == input.length() - 1) {
                if (accumulator.length() > 0) {
                    if (isInteger(accumulator)) {
                        listOfLexemes.add(
                                new Lexeme(LexemeType.LITERAL_INTEGER, accumulator.toString()));
                    } else if (isReal(accumulator)) {
                        listOfLexemes.add(
                                new Lexeme(LexemeType.LITERAL_REAL, accumulator.toString()));
                    } else if (isId(accumulator)) {
                        final String id = accumulator.toString().toLowerCase();
                        if (id.equals("true") || id.equals("false")) {
                            listOfLexemes.add(
                                    new Lexeme(LexemeType.LITERAL_BOOLEAN, id));
                        } else if (id.equals("sin")) {
                            listOfLexemes.add(
                                    new Lexeme(LexemeType.KEYWORD_SIN, id));
                        } else if (id.equals("if")) {
                            listOfLexemes.add(
                                    new Lexeme(LexemeType.KEYWORD_IF, id));
                        } else if (id.equals("then")) {
                            listOfLexemes.add(
                                    new Lexeme(LexemeType.KEYWORD_THEN, id));
                        } else if (id.equals("else")) {
                            listOfLexemes.add(
                                    new Lexeme(LexemeType.KEYWORD_ELSE, id));
                        } else if (id.equals("for")) {
                            listOfLexemes.add(
                                    new Lexeme(LexemeType.KEYWORD_FOR, id));
                        } else if (id.equals("to")) {
                            listOfLexemes.add(
                                    new Lexeme(LexemeType.KEYWORD_TO, id));
                        } else if (id.equals("do")) {
                            listOfLexemes.add(
                                    new Lexeme(LexemeType.KEYWORD_DO, id));
                        } else if (id.equals("begin")) {
                            listOfLexemes.add(
                                    new Lexeme(LexemeType.KEYWORD_BEGIN, id));
                        } else if (id.equals("end")) {
                            listOfLexemes.add(
                                    new Lexeme(LexemeType.KEYWORD_END, id));
                        } else if (id.equals("integer")) {
                            listOfLexemes.add(
                                    new Lexeme(LexemeType.KEYWORD_INTEGER, id));
                        } else if (id.equals("real")) {
                            listOfLexemes.add(
                                    new Lexeme(LexemeType.KEYWORD_REAL, id));
                        } else if (id.equals("boolean")) {
                            listOfLexemes.add(
                                    new Lexeme(LexemeType.KEYWORD_BOOLEAN, id));
                        } else {
                            listOfLexemes.add(
                                    new Lexeme(LexemeType.ID, id));
                        }
                    } else {
                        System.out.println(" Lexical Analyzer Error : " + "Internal error at index " + i + ".");
                        return null;
                    }

                    accumulator = new StringBuilder();
                }

                if (lexemeToAdd != null) {
                    listOfLexemes.add(lexemeToAdd);
                    lexemeToAdd = null;
                }
            }
        }

        return listOfLexemes;
    }

    private boolean isId(StringBuilder lexeme) {
        for (int i = 0; i < lexeme.length(); i++) {
            final char c = lexeme.charAt(i);
            if (!(isDigit(c) || isLetter(c))) {
                return false;
            }
        }

        if (!isLetter(lexeme.charAt(0))) {
            System.out.println(" Lexical Analyzer Error : " + "Invalid Id: '" + lexeme + "'. First char must be a letter.");
            return false;
        }

        return true;
    }

    private boolean isInteger(StringBuilder lexeme) {
        for (int i = 0; i < lexeme.length(); i++) {
            if (!isDigit(lexeme.charAt(i))) {
                return false;
            }
        }

        return true;
    }

    private boolean isReal(StringBuilder lexeme) {
        for (int i = 0; i < lexeme.length(); i++) {
            final char c = lexeme.charAt(i);
            if (!(isDigit(c) || c == '.')) {
                return false;
            }
        }

        return containsOnePeriod(lexeme);
    }

    private boolean isDigit(char c) {
        return (c >= '0') && (c <= '9');
    }

    private boolean isLetter(char c) {
        return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
    }

    private boolean containsOnePeriod(StringBuilder input) {
        boolean periodFound = false;
        for (int i = 0; i < input.length(); i++) {
            if (input.charAt(i) == '.') {
                if (periodFound) {
                    System.out.println(" Lexical Analyzer Error : " + "Invalid usage of >= 2 '.' symbols.");
                    return false;
                }
                periodFound = true;
            }
        }

        return periodFound;
    }
}

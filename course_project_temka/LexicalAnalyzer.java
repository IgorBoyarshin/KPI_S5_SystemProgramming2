import java.util.ArrayList;
import java.util.List;

public class LexicalAnalyzer {

    public LexicalAnalyzer() {

    }

    public List<Token> analyze(String input) {
        List<Token> listOfTokens = new ArrayList<>();
        StringBuilder accumulator = new StringBuilder();
        boolean foundSeparator;

        for (int i = 0; i < input.length(); i++) {
            final char c = input.charAt(i);
            final boolean existsNextChar = (i + 1) < input.length();
            Token tokenToAdd = null;

            switch (c) {
                case '[':
                    tokenToAdd = new Token(TokenType.BracketsOpen, "[");
                    foundSeparator = true;
                    break;
                case ']':
                    tokenToAdd = new Token(TokenType.BracketsClose, "]");
                    foundSeparator = true;
                    break;
                case '(':
                    tokenToAdd = new Token(TokenType.ParenthesesOpen, "(");
                    foundSeparator = true;
                    break;
                case ')':
                    tokenToAdd = new Token(TokenType.ParenthesesClose, ")");
                    foundSeparator = true;
                    break;
                case '{':
                    tokenToAdd = new Token(TokenType.BracesOpen, "{");
                    foundSeparator = true;
                    break;
                case '}':
                    tokenToAdd = new Token(TokenType.BracesClose, "}");
                    foundSeparator = true;
                    break;

                case ';':
                    tokenToAdd = new Token(TokenType.Semicolon, ";");
                    foundSeparator = true;
                    break;
                case ',':
                    tokenToAdd = new Token(TokenType.Comma, ",");
                    foundSeparator = true;
                    break;

                case '+':
                    tokenToAdd = new Token(TokenType.OperatorPlus, "+");
                    foundSeparator = true;
                    break;
                case '-':
                    tokenToAdd = new Token(TokenType.OperatorMinus, "-");
                    foundSeparator = true;
                    break;
                case '*':
                    tokenToAdd = new Token(TokenType.OperatorMultiply, "*");
                    foundSeparator = true;
                    break;
                case '/':
                    tokenToAdd = new Token(TokenType.OperatorDivide, "/");
                    foundSeparator = true;
                    break;

                case '>':
                    if (existsNextChar && (input.charAt(i + 1) == '=')) {
                        tokenToAdd = new Token(TokenType.OperatorGreaterOrEquals, ">=");
                        i++;
                    } else {
                        tokenToAdd = new Token(TokenType.OperatorGreater, ">");
                    }
                    foundSeparator = true;
                    break;
                case '<':
                    if (existsNextChar && (input.charAt(i + 1) == '=')) {
                        tokenToAdd = new Token(TokenType.OperatorLessOrEquals, "<=");
                        i++;
                    } else {
                        tokenToAdd = new Token(TokenType.OperatorLess, "<");
                    }
                    foundSeparator = true;
                    break;

                case '!':
                    if (existsNextChar && (input.charAt(i + 1) == '=')) {
                        tokenToAdd = new Token(TokenType.OperatorNotEquals, "!=");
                        i++;
                    } else {
                        System.out.println("*** Lexical Analyzer Error ***: " + "Invalid token at " + i + ": " + c + ".");
                        return null;
                    }
                    foundSeparator = true;
                    break;

                case '=':
                    if (existsNextChar && (input.charAt(i + 1) == '=')) {
                        tokenToAdd = new Token(TokenType.OperatorEquals, "==");
                        i++;
                    } else {
                        tokenToAdd = new Token(TokenType.OperatorAssign, "=");
                    }
                    foundSeparator = true;
                    break;

                case ' ':
                    foundSeparator = true;
                    break;

                default:
                    if (!(isDigit(c) || isLetter(c) || c == '.')) {
                        System.out.println("*** Lexical Analyzer Error ***: " + "Invalid char at " + i + ": " + c + ".");
                        return null;
                    }

                    foundSeparator = false;
                    accumulator.append(c);
            }

            if (foundSeparator || i == input.length() - 1) {
                if (accumulator.length() > 0) {
                    if (isInt(accumulator)) {
                        listOfTokens.add(
                                new Token(TokenType.LiteralInt, accumulator.toString()));
                    } else if (isFloat(accumulator)) {
                        listOfTokens.add(
                                new Token(TokenType.LiteralFloat, accumulator.toString()));
                    } else if (isId(accumulator)) {
                        final String id = accumulator.toString();
                        if (id.equals("true") || id.equals("false")) {
                            listOfTokens.add(
                                    new Token(TokenType.LiteralBool, id));
                        } else if (id.equals("if")) {
                            listOfTokens.add(
                                    new Token(TokenType.KeywordIf, id));
                        } else if (id.equals("else")) {
                            listOfTokens.add(
                                    new Token(TokenType.KeywordElse, id));
                        } else if (id.equals("int")) {
                            listOfTokens.add(
                                    new Token(TokenType.KeywordInt, id));
                        } else if (id.equals("float")) {
                            listOfTokens.add(
                                    new Token(TokenType.KeywordFloat, id));
                        } else if (id.equals("bool")) {
                            listOfTokens.add(
                                    new Token(TokenType.KeywordBool, id));
                        } else {
                            listOfTokens.add(
                                    new Token(TokenType.Id, id));
                        }
                    } else {
                        System.out.println("*** Lexical Analyzer Error ***: " + "Internal error at index " + i + ".");
                        return null;
                    }

                    accumulator = new StringBuilder();
                }

                if (tokenToAdd != null) {
                    listOfTokens.add(tokenToAdd);
                    tokenToAdd = null;
                }
            }
        }

        return listOfTokens;
    }

    private boolean isId(StringBuilder token) {
        for (int i = 0; i < token.length(); i++) {
            final char c = token.charAt(i);
            if (!(isDigit(c) || isLetter(c))) {
                return false;
            }
        }

        if (!isLetter(token.charAt(0))) {
            System.out.println("*** Lexical Analyzer Error ***: " + "Invalid Id: '" + token + "'. First char must be a letter.");
            return false;
        }

        return true;
    }

    private boolean isInt(StringBuilder token) {
        for (int i = 0; i < token.length(); i++) {
            if (!isDigit(token.charAt(i))) {
                return false;
            }
        }

        return true;
    }

    private boolean isFloat(StringBuilder token) {
        for (int i = 0; i < token.length(); i++) {
            final char c = token.charAt(i);
            if (!(isDigit(c) || c == '.')) {
                return false;
            }
        }

        return containsOnePeriod(token);
    }

    private boolean isDigit(char c) {
        return (c >= '0') && (c <= '9');
    }

    private boolean isLetter(char c) {
        return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
    }

    private boolean isBoolLiteral(String input) {
        return (input.equals("true") || input.equals("false"));
    }

    private boolean containsOnePeriod(StringBuilder input) {
        boolean periodFound = false;
        for (int i = 0; i < input.length(); i++) {
            if (input.charAt(i) == '.') {
                if (periodFound) {
                    System.out.println("*** Lexical Analyzer Error ***: " + "Invalid usage of >= 2 '.' symbols.");
                    return false;
                }
                periodFound = true;
            }
        }

        return periodFound;
    }
}

import java.util.ArrayList;
import java.util.List;

public class LexicalAnalyzer {
    private List<Lexeme> lexemeTable;

    public LexicalAnalyzer() {
        this.lexemeTable = new ArrayList<>();
    }

    public List<Lexeme> analyze(String input) {
        final int length = input.length();
        if (length == 0) {
            return lexemeTable;
        }

        int currentIndex = 0;
        StringBuilder accumulator = new StringBuilder("");
        boolean currentIsNumber = false;
        while (currentIndex < length) {
            String c = input.charAt(currentIndex) + "";
            LexemeType lexemeType = null;
            boolean separatorFound = false;

            switch (c) {
                case "(":
                case ")":
                    lexemeType = LexemeType.Parentheses;
                    separatorFound = true;
                    break;

                case "{":
                case "}":
                    lexemeType = LexemeType.CurlyBraces;
                    separatorFound = true;
                    break;

                case "[":
                case "]":
                    lexemeType = LexemeType.SquareBrackets;
                    separatorFound = true;
                    break;

                case "+":
                    if (currentIndex < length + 1
                            && input.charAt(currentIndex + 1) == '+') {
                        c += '+';
                        currentIndex++;
                    }
                case "-":
                    if (currentIndex < length + 1
                            && input.charAt(currentIndex + 1) == '-') {
                        c += '-';
                        currentIndex++;
                    }
                case "*":
                case "/":
                    if (currentIndex < length + 1
                            && input.charAt(currentIndex + 1) == '=') {
                        c += '=';
                        currentIndex++;
                    }
                case "=":
                    lexemeType = LexemeType.Operator;
                    separatorFound = true;
                    break;

                case ";":
                    lexemeType = LexemeType.Semicolon;
                    separatorFound = true;
                    break;

                case " ":
                    separatorFound = true;
                    break;

                default:
                    separatorFound = false;
                    if (isAlpha(c.charAt(0))) {
                        accumulator.append(c);

                        if (accumulator.length() == 1) {
                            currentIsNumber = false;
                        } else if (currentIsNumber) {
                            int index = currentIndex + 1;
                            for (; index < length; index++) {
                                final char symbol = input.charAt(index);
                                if (isAlpha(symbol) || isNumber(symbol)) {
                                    accumulator.append(symbol);
                                } else {
                                    break;
                                }
                            }

                            currentIndex = index - 1;

                            System.out.println("!!! Invalid literal at char "
                                    + currentIndex + ": " + accumulator.toString());
                            separatorFound = true;
                            lexemeType = LexemeType.Unknown;
                            c = " "; // so that it's not added to the table
                        }

                    } else if (isNumber(c.charAt(0))) {
                        if (accumulator.length() == 0) {
                            currentIsNumber = true;
                        }

                        accumulator.append(c);
                    } else {
                        System.out.println("!!! Unknown char at "
                                + currentIndex + ": " + c);
                        lexemeType = LexemeType.Unknown;
                        separatorFound = true;
                    }
            }

            if (separatorFound) {
                final boolean accumulating = accumulator.length() > 0;
                if (accumulating) {
                    if (currentIsNumber) {
                        lexemeTable.add(
                                new Lexeme(lexemeType == LexemeType.Unknown ? 
                                    lexemeType : 
                                    LexemeType.Literal, accumulator.toString()));
                    } else {
                        lexemeTable.add(new Lexeme(LexemeType.Identifier, accumulator.toString()));
                    }

                    accumulator = new StringBuilder();
                }

                if (c.charAt(0) != ' ') {
                    lexemeTable.add(new Lexeme(lexemeType, c + ""));
                }
            }

            currentIndex++;
        }

        return lexemeTable;
    }

    private boolean isAlpha(char c) {
        return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
    }

    private boolean isNumber(char c) {
        return (c >= '0' && c <= '9');
    }

    public void print() {
        System.out.println("Lexeme table:");
        for (Lexeme lexeme : lexemeTable) {
            System.out.println(lexeme);
        }
    }

    boolean insert(int index, Lexeme lexeme) {
        if (index >= lexemeTable.size()) {
            return false;
        }

        lexemeTable.add(index, lexeme);

        return true;
    }


    boolean update(int index, Lexeme lexeme) {
        if (index >= lexemeTable.size()) {
            return false;
        }

        lexemeTable.set(index, lexeme);

        return true;
    }


    Lexeme select(int index) {
        if (index >= lexemeTable.size()) {
            return null;
        }

        return lexemeTable.get(index);
    }
}

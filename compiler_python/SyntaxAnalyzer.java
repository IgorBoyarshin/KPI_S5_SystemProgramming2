// package CourseWork;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class SyntaxAnalyzer {

    private Map<String, Type> variablesTable;
    private List<Node> listOfNodes;

    // For creating labels
    private static int nextLabelNumberToUse = 0;

    public SyntaxAnalyzer() {
        variablesTable = new HashMap<>();
    }

    public boolean parse(List<Token> listOfTokens) {
        List<List<Token>> lines = split(listOfTokens, TokenName.NEW_LINE);
        listOfNodes = new ArrayList<>();

        boolean insideFunction = false;
        String currentFunctionName = "";
        int amountOfFunctionArguments = 0;

        int lineIndex = -1;
        for (List<Token> line : lines) {
            lineIndex++;

            final int shiftLevel = getShiftLevel(line);
            final List<Token> strippedLine = strip(line);

            // The line is empty
            if (strippedLine.size() == 0) {
                if (insideFunction) {
                    // The function has ended, create an appropriate Node
                    listOfNodes.add(
                        new FunctionEnd(currentFunctionName, amountOfFunctionArguments)
                    );
                }
                insideFunction = false;
                // if (!ifStatementMet) {
                    continue;
                // }
            }


            final TokenName firstTokenName = strippedLine.get(0).getTokenName();

            if (ifStatementMet && shiftLevel <= ifStatementShift) {
                System.out.println("yeaaa " + lineIndex);
                if (!(firstTokenName == TokenName.KEYWORD_ELIF || firstTokenName == TokenName.KEYWORD_ELSE)) {
                    listOfNodes.add(new Statement(
                        "ifGlEnd" + ifStatementGlobalEnd + ":\n"
                    ));

                    ifStatementGlobalEnd = -1;
                    ifStatementMet = false;
                    ifStatementShift = -1;
                    ifStatementEndLabelIndex = -1;
                }
            }
            //

            switch (firstTokenName) {
                case KEYWORD_DEF:
                    {
                        if (insideFunction || shiftLevel > 0) {
                            System.out.println("Syntax Error: Invalid function definition. " +
                                    "Check indentation or scope. In line " + lineIndex + ".");
                            return false;
                        }
                        if (strippedLine.size() < 5) {
                            System.out.println("Syntax Error: Invalid function definition. " +
                                    "Too few tokens. " + "In line " + lineIndex + ".");
                            return false;
                        }
                        insideFunction = true;

                        final Token tokenID = strippedLine.get(1);
                        final TokenName tokenIdName = tokenID.getTokenName();
                        final String tokenIdValue = tokenID.getTokenValue();
                        if (tokenIdName != TokenName.IDENTIFIER) {
                            System.out.println("Syntax Error: Invalid function definition. " +
                                    "Expected IDENTIFIER after 'def'. " + "In line " + lineIndex + ".");
                            return false;
                        }
                        if (variablesTable.containsKey(tokenIdValue)) {
                            System.out.println("Syntax Error: Invalid function definition. " +
                                    "Function redefinition. " + "In line " + lineIndex + ".");
                            return false;
                        }
                        currentFunctionName = tokenIdValue;

                        final char firstChar = tokenIdValue.charAt(0);
                        final Type functionType = (firstChar == 'b') ?
                            Type.FUNC_BOOLEAN : Type.FUNC_NUM;

                        variablesTable.put(tokenIdValue, functionType);

                        if (!strippedLine.get(2).getTokenValue().equals("(")) {
                            System.out.println("Syntax Error: Invalid function definition. " +
                                    "Expected '(' after function name. " + "In line " + lineIndex + ".");
                            return false;
                        }

                        int iterator;
                        boolean expectID = true;
                        List<Argument> arguments = new ArrayList<>();
                        for (iterator = 3; iterator < strippedLine.size() - 2; iterator++) {
                            final String argumentValue = strippedLine.get(iterator).getTokenValue();
                            final TokenName argumentName = strippedLine.get(iterator).getTokenName();

                            if (expectID) {
                                if (argumentName != TokenName.IDENTIFIER) {
                                    System.out.println("Syntax Error: Invalid function definition. " +
                                            "Expected IDENTIFIER as an argument, but found " + argumentName +
                                            " instead. " + "In line " + lineIndex + ".");
                                    return false;
                                }
                                if (variablesTable.containsKey(argumentValue)) {
                                    System.out.println("Syntax Error: Invalid function definition. " +
                                        "Variable redefinition: " + argumentValue + ". " + "In line " + lineIndex + ".");
                                    return false;
                                }

                                final char firstCharOfVar = argumentValue.charAt(0);
                                final Type argumentType = (firstCharOfVar == 'b') ?
                                    Type.BOOLEAN : Type.NUM;

                                arguments.add(new Argument(argumentValue, argumentType));
                                variablesTable.put(argumentValue, argumentType);

                                expectID = false;
                            } else {
                                switch (argumentValue) {
                                    case (","):
                                        expectID = true;
                                        break;
                                    default:
                                        // We can't have reached ')', because it is supposed to
                                        // have index (strippedLine.size() - 2), but the loop doesn't
                                        // go up to that value. So an error for sure
                                        System.out.println("Syntax Error: Invalid function definition. " +
                                            "Premature end of function declaration. Expected ',', but found " +
                                            argumentValue + " instead. " + "In line " + lineIndex + ".");
                                        return false;
                                }
                            }
                        }

                        if (!strippedLine.get(strippedLine.size() - 1).getTokenValue().equals(":")) {
                            System.out.println("Syntax Error: Invalid function definition. " +
                                "Expected ':' at the end of function declaration. " + "In line " + lineIndex + ".");
                            return false;
                        }
                        amountOfFunctionArguments = arguments.size();

                        listOfNodes.add(new FunctionNew(currentFunctionName, arguments));
                        // functionsTable.put(currentFunctionName, arguments);
                    }
                    break;

                case KEYWORD_RETURN:
                    {
                        if (!insideFunction) {
                            System.out.println("Syntax Error: Unexpected 'return' keyword: " +
                                "Not in function. " + "In line " + lineIndex + ".");
                            return false;
                        }
                        if (strippedLine.size() == 1) {
                            listOfNodes.add(new Statement(
                                "\tjmp " + currentFunctionName + "End\n"
                            ));
                        } else {
                            final Piece reducedExpression = reduceExpression(strippedLine, 1, lineIndex);
                            if (reducedExpression == null) {
                                System.out.println("Syntax Error: Invalid return expression. " +
                                    "In line " + lineIndex + ".");
                                return false;
                            }

                            listOfNodes.add(new Statement(
                                reducedExpression.content + // already in EAX
                                "\tjmp " + currentFunctionName + "End\n"
                            ));
                        }
                    }
                    break;

                case IDENTIFIER:
                    {
                        final String idValue = strippedLine.get(0).getTokenValue();
                        final TokenName secondTokenName = strippedLine.get(1).getTokenName();
                        switch (secondTokenName) {
                            case ASSIGN:
                                if (!variablesTable.containsKey(idValue)) {
                                    final Type idType = idValue.charAt(0) == 'b' ? Type.BOOLEAN : Type.NUM;
                                    variablesTable.put(idValue, idType);
                                }

                                final Piece reducedExpression =
                                    reduceExpression(strippedLine, 2, lineIndex);
                                if (reducedExpression == null) {
                                    System.out.println("Syntax Error: Invalid expression. " +
                                        "In line " + lineIndex + ".");
                                    return false;
                                }

                                final Type variableType = variablesTable.get(idValue);
                                if (variableType == Type.BOOLEAN &&
                                        reducedExpression.name != PieceName.Bool) {
                                    System.out.println("Syntax Error: Incompatible types on the left and right hand side of '=': " +
                                        "Boolean and " + reducedExpression.name + ". " +
                                        "In line " + lineIndex + ".");
                                    return false;
                                }
                                if (variableType == Type.NUM &&
                                        reducedExpression.name != PieceName.Number) {
                                    System.out.println("Syntax Error: Incompatible types on the left and right hand side of '=': " +
                                        "Number and " + reducedExpression.name + ". " +
                                        "In line " + lineIndex + ".");
                                    return false;
                                }

                                listOfNodes.add(new Statement(
                                    reducedExpression.content +
                                    "\tmov " + idValue + ", eax\n" +
                                    "\n"
                                ));
                                break;
                            // case LEFT_PARENTHESIS: // void function call
                                // TODO
                            default:
                                System.out.println("Syntax Error: Invalid token after ID. " +
                                    "Expected '(' or '='. " + "In line " + lineIndex + ".");
                                return false;
                        }
                    }
                    break;


                // case KEYWORD_IF:
                //     {
                //         if (ifStatementMet) {
                //         // if (ifStatementMet && shiftLevel > ifStatementShift) {
                //             System.out.println("Syntax Error: " +
                //                 "Nested If statements not allowed. " + "In line " + lineIndex + ".");
                //             return false;
                //         }
                //         if ((insideFunction && shiftLevel != 1) || (!insideFunction && shiftLevel != 0)) {
                //             System.out.println("Syntax Error: " +
                //                 "Bad indentation for If stateent. " + "In line " + lineIndex + ".");
                //             return false;
                //         }
                //         if (strippedLine.get(strippedLine.size() - 1).getTokenName() != TokenName.COLON) {
                //             System.out.println("Syntax Error: " +
                //                 "Expected ':' at the end of If statement. " + "In line " + lineIndex + ".");
                //             return false;
                //         }
                //
                //         ifStatementMet = true;
                //         ifStatementFinished = false;
                //         ifStatementShift = shiftLevel;
                //         ifStatementEndLabelIndex = nextLabelNumberToUse++;
                //         ifStatementGlobalEnd = nextLabelNumberToUse++;
                //
                //         final Piece reducedExpression = reduceExpression(strippedLine, 1, lineIndex);
                //
                //         listOfNodes.add(new Statement(
                //             reducedExpression.content +
                //             "\tcmp eax, 1\n" +
                //             "\tjne IfEnd" + ifStatementEndLabelIndex + "\n"
                //         ));
                //     }
                //     break;
                //
                // case KEYWORD_ELIF:
                //     {
                //         if (ifStatementFinished) {
                //             System.out.println("Syntax Error: " +
                //                 "Unexpected 'elif' statement: no If started. " + "In line " + lineIndex + ".");
                //             return false;
                //         }
                //         if (!ifStatementMet) {
                //             System.out.println("Syntax Error: " +
                //                 "Unexpected 'elif' keyword outside an If statement. " + "In line " + lineIndex + ".");
                //             return false;
                //         }
                //         if (shiftLevel != ifStatementShift) {
                //             System.out.println("Syntax Error: " +
                //                 "Invalid shift level for 'elif'. " + "In line " + lineIndex + ".");
                //             return false;
                //         }
                //         if ((insideFunction && shiftLevel != 1) || (!insideFunction && shiftLevel != 0)) {
                //             System.out.println("Syntax Error: " +
                //                 "Bad indentation for 'elif'. " + "In line " + lineIndex + ".");
                //             return false;
                //         }
                //         if (strippedLine.get(strippedLine.size() - 1).getTokenName() != TokenName.COLON) {
                //             System.out.println("Syntax Error: " +
                //                 "Expected ':' at the end of If statement. " + "In line " + lineIndex + ".");
                //             return false;
                //         }
                //
                //         listOfNodes.add(new Statement(
                //             "\tjmp ifGlobalEnd" + ifStatementGlobalEnd + "\n" +
                //             "ifEnd" + ifStatementEndLabelIndex + ":\n"
                //         ));
                //
                //         ifStatementEndLabelIndex = nextLabelNumberToUse++;
                //         final Piece reducedExpression = reduceExpression(strippedLine, 1, lineIndex);
                //
                //         listOfNodes.add(new Statement(
                //             reducedExpression.content +
                //             "\tcmp eax, 1\n" +
                //             "\tjne IfEnd" + ifStatementEndLabelIndex + "\n"
                //         ));
                //     }
                //     break;
                //
                // case KEYWORD_ELSE:
                //     {
                //         if (ifStatementFinished) {
                //             System.out.println("Syntax Error: " +
                //                 "Unexpected 'else' statement: no If started. " + "In line " + lineIndex + ".");
                //             return false;
                //         }
                //         if (!ifStatementMet) {
                //             System.out.println("Syntax Error: " +
                //                 "Unexpected 'else' keyword outside an If statement. " + "In line " + lineIndex + ".");
                //             return false;
                //         }
                //         if (shiftLevel != ifStatementShift) {
                //             System.out.println("Syntax Error: " +
                //                 "Invalid shift level for 'else'. " + "In line " + lineIndex + ".");
                //             return false;
                //         }
                //         if ((insideFunction && shiftLevel != 1) || (!insideFunction && shiftLevel != 0)) {
                //             System.out.println("Syntax Error: " +
                //                 "Bad indentation for 'else'. " + "In line " + lineIndex + ".");
                //             return false;
                //         }
                //         if (strippedLine.size() != 2 || strippedLine.get(1).getTokenName() != TokenName.COLON) {
                //             System.out.println("Syntax Error: " +
                //                 "Invalid syntax for 'else' clause. Must be only 'else:' in line. " + "In line " + lineIndex + ".");
                //             return false;
                //         }
                //
                //         listOfNodes.add(new Statement(
                //             "\tjmp ifGlobalEnd" + ifStatementGlobalEnd + "\n" +
                //             "ifEnd" + ifStatementEndLabelIndex + ":\n"
                //         ));
                //
                //         ifStatementFinished = true;
                //
                //         // ifStatementMet = false;
                //         // ifStatementShift = -1;
                //         // ifStatementEndLabelIndex = -1;
                //
                //         // listOfNodes.add(new Statement(
                //         //     reducedExpression.content
                //         // ));
                //     }
                //     break;

                default:
                    System.out.println("Syntax Error: Invalid starting token. " +
                        "In line " + lineIndex + ".");
                    return false;
            }
        }

        return true;
    }


    // Call me when you have:
    // - ID_VAR = .... in which case give me the index of the first token of ....
    // - ID_FUNC .... in which case give me the index of ID_FUNC(so, zero)
    // - return .... in which case give me the index of the first token of ....
    // If successsfull, returns one Piece containing all the code.
    // Otherwise returns null.
    private Piece reduceExpression(List<Token> line, int startIndex, int lineIndex) {
        // startIndex = index of first symbol(be it an ID, an ID of a function, a '(' or a literal)

        // final boolean commasAreCorrect = areCommasCorrect(line);
        // if (!commasAreCorrect) {
        //     System.out.println("Syntax Error: Invalid commas. " +
        //         "In line " + lineIndex + ".");
        //     return null;
        // }

        final boolean parenthesesAreCorrect = areParenthesesCorrect(line);
        if (!parenthesesAreCorrect) {
            System.out.println("Syntax Error: Incorrect patentheses. " +
                "In line " + lineIndex + ".");
            return null;
        }

        final List<Piece> pieces = convertTokensIntoPieces(line, startIndex, lineIndex);
        if (pieces == null) {
            return null;
        }
        // System.out.println("Pieces:");
        // for (Piece piece : pieces) {
        //     System.out.println(":: " + piece.name.name());
        // }

        boolean somethingChanged;
        do {
            somethingChanged = false;

            final int simpleParenthesesIndex = findSimpleParentheses(pieces);
            if (simpleParenthesesIndex == -2) {
                return null;
            } else if (simpleParenthesesIndex >= 0) {
                pieces.remove(simpleParenthesesIndex + 2);
                pieces.remove(simpleParenthesesIndex + 0);
                somethingChanged = true;
                continue;
            } // else == -1 => not found => continue with other patterns


            final int operatorApplicationIndex = findOperator(pieces, lineIndex);
            if (operatorApplicationIndex == -2) {
                return null;
            } else if (operatorApplicationIndex >= 0) {
                final Piece left = pieces.get(operatorApplicationIndex - 1);
                final Piece operator = pieces.get(operatorApplicationIndex);
                final Piece right = pieces.get(operatorApplicationIndex + 1);

                final Piece newPiece = new Piece(
                        PieceName.getOperatorResultType(operator.name),
                        right.content +
                        "\tpush eax\n" +
                        left.content +
                        "\tpop ebx\n" +
                        operator.content
                        // + "\n"
                );

                pieces.remove(operatorApplicationIndex + 1);
                pieces.remove(operatorApplicationIndex + 0);
                pieces.set(operatorApplicationIndex - 1, newPiece);

                somethingChanged = true;
                continue;
            } // else == -1 => not found => continue with other patterns


            final int commaExprFuncEndIndex = findCommaExprFuncEnd(pieces);
            if (commaExprFuncEndIndex != -1) {
                final String secondContent = pieces.get(commaExprFuncEndIndex + 1).content;
                final String thirdContent = pieces.get(commaExprFuncEndIndex + 2).content;

                final Piece newPiece = new Piece(
                        PieceName.FunctionEnd,
                        thirdContent +
                        secondContent +
                        "\tpush eax\n"
                );

                pieces.remove(commaExprFuncEndIndex + 2);
                pieces.remove(commaExprFuncEndIndex + 1);
                pieces.set(commaExprFuncEndIndex, newPiece);

                somethingChanged = true;
                continue;
            }


            final int exprFuncEndIndex = findExprFuncEnd(pieces);
            if (exprFuncEndIndex != -1) {
                final Piece first = pieces.get(exprFuncEndIndex + 0);
                final Piece second = pieces.get(exprFuncEndIndex + 1);

                final Piece newPiece = new Piece(
                        PieceName.FunctionEnd,
                        second.content +
                        first.content +
                        "\tpush eax\n"
                );

                pieces.remove(exprFuncEndIndex + 1);
                pieces.set(exprFuncEndIndex, newPiece);

                somethingChanged = true;
                continue;
            }


            final int funcBeginFuncEndIndex = findFuncBeginFuncEnd(pieces);
            if (funcBeginFuncEndIndex != -1) {
                final Piece first = pieces.get(funcBeginFuncEndIndex + 0);
                final Piece second = pieces.get(funcBeginFuncEndIndex + 1);

                Piece newPiece = null;
                if (first.name == PieceName.FunctionBool) {
                    newPiece = new Piece(
                            PieceName.Bool,
                            second.content +
                            first.content
                            + "\n"
                    );
                } else if (first.name == PieceName.FunctionNumber) {
                    newPiece = new Piece(
                            PieceName.Number,
                            second.content +
                            first.content
                            + "\n"
                    );
                }

                pieces.remove(funcBeginFuncEndIndex + 1);
                pieces.set(funcBeginFuncEndIndex, newPiece);

                somethingChanged = true;
                continue;
            }

        } while (somethingChanged);

        if (pieces.size() == 1) {
            return pieces.get(0);
        } else {
            System.out.println("*** errrr");
            return null;
        }
    }


    // -1 === not found
    private int findSimpleParentheses(List<Piece> line) {
        for (int i = 0; i < line.size() - 2; i++) {
            final PieceName leftName = line.get(i).name;
            final PieceName exprName = line.get(i+1).name;
            final PieceName rightName = line.get(i+2).name;

            if (leftName == PieceName.ParenthesesOpen &&
                    rightName == PieceName.ParenthesesClose) {
                if (exprName == PieceName.Number || exprName == PieceName.Bool) {
                    return i;
                }
            }
        }

        return -1;
    }


    // -1 === not found
    private int findCommaExprFuncEnd(List<Piece> line) {
        for (int i = 0; i < line.size() - 2; i++) {
            final PieceName first= line.get(i).name;
            final PieceName second= line.get(i+1).name;
            final PieceName third = line.get(i+2).name;

            if (first == PieceName.Comma && third == PieceName.FunctionEnd) {
                if (second == PieceName.Number || second == PieceName.Bool) {
                    return i;
                }
            }
        }

        return -1;
    }


    // -1 === not found
    private int findExprFuncEnd(List<Piece> line) {
        for (int i = 0; i < line.size() - 1; i++) {
            final Piece first= line.get(i);
            final Piece second= line.get(i+1);

            if (second.name == PieceName.FunctionEnd) {
                if (first.name == PieceName.Bool || first.name == PieceName.Number) {
                    return i;
                }
            }
        }

        return -1;
    }


    // -1 === not found
    private int findFuncBeginFuncEnd(List<Piece> line) {
        for (int i = 0; i < line.size() - 1; i++) {
            final Piece first= line.get(i);
            final Piece second= line.get(i+1);

            if (second.name == PieceName.FunctionEnd) {
                if (first.name == PieceName.FunctionBool ||
                        first.name == PieceName.FunctionNumber) {
                    return i;
                }
            }
        }

        return -1;
    }


    // returns the index in line where there is a valid operator application,
    // -1 === not found
    // -2 === error found
    private int findOperator(List<Piece> line, int lineIndex) {
        for (int i = 1; i < line.size() - 1; i++) {
            final Piece piece = line.get(i);
            final PieceName leftName = line.get(i - 1).name;
            final PieceName rightName = line.get(i + 1).name;
            switch (piece.name) {
                case Plus:
                case Minus:
                case Star:
                case Slash:
                case Greater:
                case GreaterOrEquals:
                case Less:
                case LessOrEquals:
                    {
                        if (leftName == PieceName.Number && rightName == PieceName.Number) {
                            return i;
                        }

                        switch (leftName) {
                            case Bool:
                                System.out.println("Semantic Error: Invalid " +
                                    "left-hand side type for operator " +
                                    piece.name + ": Bool. Expected Number. " +
                                    "In line " + lineIndex + ".");
                                return -2;
                            case ParenthesesClose:
                            case FunctionEnd:
                            case Number:
                                // allowed
                                break;
                            default:
                                System.out.println("Semantic Error: Invalid " +
                                    "left-hand side token for operator " +
                                    piece.name + ": " + leftName + ". " +
                                    "In line " + lineIndex + ".");
                                return -2;
                        }

                        switch (rightName) {
                            case Bool:
                                System.out.println("Semantic Error: Invalid " +
                                    "right-hand side type for operator " +
                                    piece.name + ": Bool. Expected Number. " +
                                    "In line " + lineIndex + ".");
                                return -2;
                            case ParenthesesOpen:
                            case FunctionNumber:
                            case Number:
                                // allowed
                            case FunctionBool:
                                // Let the error be raised later, when the function reduced into Bool
                                break;
                            default:
                                System.out.println("Semantic Error: Invalid " +
                                    "right-hand side token for operator " +
                                    piece.name + ": " + rightName + ". " +
                                    "In line " + lineIndex + ".");
                                return -2;
                        }
                    }
                    break;

                case And:
                case Or:
                    {
                        if (leftName == PieceName.Bool && rightName == PieceName.Bool) {
                            return i;
                        }

                        switch (leftName) {
                            case Number:
                                System.out.println("Semantic Error: Invalid " +
                                    "left-hand side type for operator " +
                                    piece.name + ": Number. Expected Bool. " +
                                    "In line " + lineIndex + ".");
                                return -2;
                            case ParenthesesClose:
                            case FunctionEnd:
                            case Bool:
                                // allowed
                                break;
                            default:
                                System.out.println("Semantic Error: Invalid " +
                                    "left-hand side token for operator " +
                                    piece.name + ": " + leftName + ". " +
                                    "In line " + lineIndex + ".");
                                return -2;
                        }

                        switch (rightName) {
                            case Number:
                                System.out.println("Semantic Error: Invalid " +
                                    "right-hand side type for operator " +
                                    piece.name + ": Number. Expected Bool. " +
                                    "In line " + lineIndex + ".");
                                return -2;
                            case ParenthesesOpen:
                            case FunctionBool:
                            case Bool:
                                // allowed
                            case FunctionNumber:
                                // Let the error be raised later, when the function reduced into Bool
                                break;
                            default:
                                System.out.println("Semantic Error: Invalid " +
                                    "right-hand side token for operator " +
                                    piece.name + ": " + rightName + ". " +
                                    "In line " + lineIndex + ".");
                                return -2;
                        }
                    }
                    break;

                case Equals:
                    {
                        if ((leftName == PieceName.Bool && rightName == PieceName.Bool) ||
                            (leftName == PieceName.Number && rightName == PieceName.Number)) {
                            return i;
                        }

                        if ((leftName == PieceName.FunctionEnd ||
                                    leftName == PieceName.ParenthesesClose) &&
                            (rightName == PieceName.FunctionNumber ||
                                rightName == PieceName.FunctionBool ||
                                rightName == PieceName.ParenthesesOpen)) {
                            // Allowed
                        } else {
                            System.out.println("Semantic Error: Invalid " +
                                "operator application: <left> <op> <right>:" +
                                leftName + " " + piece.name + " " + rightName + ". " +
                                "In line " + lineIndex + ".");
                            return -2;
                        }
                    }
                    break;

                default:
                    // keep searching
            }
        }

        return -1;
    }


    // There is a fixed set of tokens allowed in an expression.
    // If an unexpected token is encountered => returns null.
    private List<Piece> convertTokensIntoPieces(List<Token> tokens, int startIndex, int lineIndex) {
        List<Piece> pieces = new ArrayList<>();

        // Holds the values of opennedParentheses when a function was started.
        List<Integer> whenFunctionsBegan = new ArrayList<>();
        int opennedParentheses = 0;

        for (int index = startIndex; index < tokens.size(); index++) {
            final TokenName tokenName = tokens.get(index).getTokenName();
            final String tokenValue = tokens.get(index).getTokenValue();

            switch (tokenName) {
                case NUMBER:
                    pieces.add(new Piece(PieceName.Number, "\tmov eax, " + tokenValue + "\n"));
                    break;
                case BOOL:
                    pieces.add(new Piece(PieceName.Bool, "\tmov eax, " + (tokenValue.equals("True") ? 1 : 0) + "\n"));
                    break;
                case IDENTIFIER:
                    final Type identifierType = variablesTable.get(tokenValue);
                    if (identifierType == null) {
                        System.out.println("Syntax Error: Usage of undeclared identifier: " +
                                tokenValue + ". In line " + lineIndex + ".");
                        return null;
                    }

                    switch (identifierType) {
                        case BOOLEAN:
                            if (index + 1 < tokens.size() && tokens.get(index + 1).getTokenName() == TokenName.LEFT_PARENTHESIS) {
                                System.out.println("Syntax Error: In line " + lineIndex + ": Invalid function call: '" + tokenValue + "' is not a function.");
                                return null;
                            }
                            pieces.add(new Piece(PieceName.Bool, "\tmov eax, " + tokenValue + "\n"));
                            break;
                        case NUM:
                            if (index + 1 < tokens.size() && tokens.get(index + 1).getTokenName() == TokenName.LEFT_PARENTHESIS) {
                                System.out.println("Syntax Error: In line " + lineIndex + ": Invalid function call: '" + tokenValue + "' is not a function.");
                                return null;
                            }
                            pieces.add(new Piece(PieceName.Number, "\tmov eax, " + tokenValue + "\n"));
                            break;
                        case FUNC_BOOLEAN:
                            {
                                final Token id = tokens.get(index + 1);
                                if (id.getTokenName() == TokenName.LEFT_PARENTHESIS) {
                                    if (!variablesTable.containsKey(tokenValue)) {
                                        System.out.println("Syntax Error: Usage of undefined function: " + tokenValue + ". " +
                                            "In line " + lineIndex + ".");
                                        return null;
                                    }

                                    index++; // Skip '('
                                    whenFunctionsBegan.add(opennedParentheses++);

                                    pieces.add(new Piece(PieceName.FunctionBool, "\tcall " + tokenValue + "\n"));
                                } else {
                                    System.out.println("Syntax Error: Invalid expression: " +
                                        "Expected '(' after functional ID. " + "In line " + lineIndex + ".");
                                    return null;
                                }
                            }
                            break;
                        case FUNC_NUM:
                            {
                                final Token id = tokens.get(index + 1);
                                if (id.getTokenName() == TokenName.LEFT_PARENTHESIS) {
                                    if (!variablesTable.containsKey(tokenValue)) {
                                        System.out.println("Syntax Error: Usage of undefined function: " + tokenValue + ". " +
                                            "In line " + lineIndex + ".");
                                        return null;
                                    }

                                    index++; // Skip '('
                                    whenFunctionsBegan.add(opennedParentheses++);

                                    pieces.add(new Piece(PieceName.FunctionNumber, "\tcall " + tokenValue + "\n"));
                                } else {
                                    System.out.println("Syntax Error: Invalid expression: " +
                                        "Expected '(' after functional ID. " + "In line " + lineIndex + ".");
                                    return null;
                                }
                            }
                            break;
                    }
                    break;
                case LEFT_PARENTHESIS:
                    pieces.add(new Piece(PieceName.ParenthesesOpen, ""));
                    opennedParentheses++;
                    break;
                case RIGHT_PARENTHESIS:
                    opennedParentheses--;
                    if (whenFunctionsBegan.size() > 0 &&
                            whenFunctionsBegan.get(whenFunctionsBegan.size() - 1) == opennedParentheses) {
                        // This parentheses matches the one when the function started
                        whenFunctionsBegan.remove(whenFunctionsBegan.size() - 1);

                        pieces.add(new Piece(PieceName.FunctionEnd, ""));
                    } else {
                        pieces.add(new Piece(PieceName.ParenthesesClose, ""));
                    }
                    break;
                case COMMA:
                    pieces.add(new Piece(PieceName.Comma, ""));
                    break;
                case PLUS:
                    pieces.add(new Piece(PieceName.Plus, "\tadd eax, ebx" + "\n"));
                    break;
                case MINUS:
                    pieces.add(new Piece(PieceName.Minus, "\tsub eax, ebx" + "\n"));
                    break;
                case STAR:
                    pieces.add(new Piece(PieceName.Star, "\tmul eax, ebx" + "\n"));
                    break;
                case SLASH:
                    pieces.add(new Piece(PieceName.Slash, "\tdiv eax, ebx" + "\n"));
                    break;
                case MORE:
                    pieces.add(new Piece(PieceName.Greater,
                            "\tcmp eax, ebx\n" +
                            "\tjg loop" + nextLabelNumberToUse + "\n" +
                            "\tmov eax, 0\n" +
                            "\tjmp loop" + (nextLabelNumberToUse+1) + "\n" +
                            "\tloop" + nextLabelNumberToUse++ + ":\n" +
                            "\tmov eax, 1\n" +
                            "\tloop" + nextLabelNumberToUse++ + ":\n" // +1'ed
                            ));
                    break;
                case MORE_EQUAL:
                    pieces.add(new Piece(PieceName.GreaterOrEquals,
                            "\tcmp eax, ebx\n" +
                            "\tjge loop" + nextLabelNumberToUse + "\n" +
                            "\tmov eax, 0\n" +
                            "\tjmp loop" + (nextLabelNumberToUse+1) + "\n" +
                            "\tloop" + nextLabelNumberToUse++ + ":\n" +
                            "\tmov eax, 1\n" +
                            "\tloop" + nextLabelNumberToUse++ + ":\n" // +1'ed
                            ));
                    break;
                case LESS:
                    pieces.add(new Piece(PieceName.Less,
                            "\tcmp eax, ebx\n" +
                            "\tjl loop" + nextLabelNumberToUse + "\n" +
                            "\tmov eax, 0\n" +
                            "\tjmp loop" + (nextLabelNumberToUse+1) + "\n" +
                            "\tloop" + nextLabelNumberToUse++ + ":\n" +
                            "\tmov eax, 1\n" +
                            "\tloop" + nextLabelNumberToUse++ + ":\n" // +1'ed
                            ));
                    break;
                case LESS_EQUAL:
                    pieces.add(new Piece(PieceName.LessOrEquals,
                            "\tcmp eax, ebx\n" +
                            "\tjle loop" + nextLabelNumberToUse + "\n" +
                            "\tmov eax, 0\n" +
                            "\tjmp loop" + (nextLabelNumberToUse+1) + "\n" +
                            "\tloop" + nextLabelNumberToUse++ + ":\n" +
                            "\tmov eax, 1\n" +
                            "\tloop" + nextLabelNumberToUse++ + ":\n" // +1'ed
                            ));
                    break;
                case AND:
                    pieces.add(new Piece(PieceName.And,
                            "\tand eax, ebx\n" +
                            "\tjnz loop" + nextLabelNumberToUse + "\n" +
                            "\tmov eax, 0\n" +
                            "\tjmp loop" + (nextLabelNumberToUse+1) + "\n" +
                            "\tloop" + nextLabelNumberToUse++ + ":\n" +
                            "\tmov eax, 1\n" +
                            "\tloop" + nextLabelNumberToUse++ + ":\n" // +1'ed
                            ));
                    break;
                case OR:
                    pieces.add(new Piece(PieceName.Or,
                            "\tor eax, ebx\n" +
                            "\tjnz loop" + nextLabelNumberToUse + "\n" +
                            "\tmov eax, 0\n" +
                            "\tjmp loop" + (nextLabelNumberToUse+1) + "\n" +
                            "\tloop" + nextLabelNumberToUse++ + ":\n" +
                            "\tmov eax, 1\n" +
                            "\tloop" + nextLabelNumberToUse++ + ":\n" // +1'ed
                            ));
                    break;
                case COLON:
                    return pieces; // Signal for end of expression

                default:
                    System.out.println("Syntax Error: Invalid token in expresison: " + tokenName +
                        ". In line " + lineIndex + ".");
                    return null;
            }
        }

        return pieces;
    }


    public Map<String, Type> getVariablesTable() {
        return variablesTable;
    }

    public List<Node> getListOfNodes() {
        return listOfNodes;
    }

    private List<List<Token>> split(List<Token> listOfTokens, TokenName separator) {
        List<List<Token>> lines = new ArrayList<>();

        final int size = listOfTokens.size();
        int fromIndex = 0;

        for (int i = 0; i < size; i++) {
            final TokenName currentTokenName = listOfTokens.get(i).getTokenName();
            if (currentTokenName.equals(separator)) {
                final int toIndex = i;
                lines.add(listOfTokens.subList(fromIndex, toIndex));
                fromIndex = toIndex + 1;
            }
        }

        return lines;
    }

    private int getShiftLevel(List<Token> line) {
        int amountOfShifts = 0;
        for (int i = 0; i < line.size(); i++) {
            final TokenName currentTokenName = line.get(0).getTokenName();
            if (currentTokenName == TokenName.TAB) {
                amountOfShifts++;
            } else {
                return amountOfShifts;
            }
        }

        return amountOfShifts;
    }

    private List<Token> strip(List<Token> line) {
        List<Token> strippedLine = new ArrayList<>();
        for (Token token : line) {
            final TokenName currentTokenName = token.getTokenName();
            if (currentTokenName != TokenName.TAB) {
                strippedLine.add(token);
            }
        }

        return strippedLine;
    }

    // pseudo-todo: implement other kinds of brackets
    private boolean areParenthesesCorrect(List<Token> line) {
        final int parenthesesCode = 0; // ()
        final int bracesCode = 1; // {}
        final int bracketsCode = 2; // []

        int[] stack = new int[line.size()]; // there can be no more than that elements
        int stackTop = -1;

        for (Token token : line) {
            final TokenName tokenName = token.getTokenName();
            switch (tokenName) {
                case LEFT_PARENTHESIS:
                    stack[++stackTop] = parenthesesCode;
                    break;
                case RIGHT_PARENTHESIS:
                    if (stackTop < 0 || stack[stackTop] != parenthesesCode) {
                        System.out.println("Syntax Error: Unmatched parentheses ')'. ");
                        return false;
                    }
                    stackTop--;
                    break;
                default:
            }
        }

        if (stackTop != -1) {
            switch (stack[0]) {
                case parenthesesCode:
                    System.out.println("Syntax Error: Unmatched parentheses '('. ");
                    break;
                default:
                    // TODO
            }
        }

        return (stackTop == -1);
    }


}

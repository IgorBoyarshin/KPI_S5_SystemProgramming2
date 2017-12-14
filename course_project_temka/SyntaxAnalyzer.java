import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class SyntaxAnalyzer {

    private static int availableLabel = 0;
    private Map<String, Type> registeredIds = new HashMap<>();

    private StringBuilder code = new StringBuilder();

    public StringBuilder parse(List<Token> tokens) {
        if (!areParenthesesCorrect(tokens, 0, tokens.size())) {
            // Error message is printed by the function
            return null;
        }

        int index = 0;
        while (index < tokens.size()) {
            final Token currentToken = tokens.get(index);
            final TokenType currentTokenType = currentToken.type;
            switch (currentTokenType) {
                case KeywordInt:
                    {
                        final int declarationResultingIndex = processVariableDeclaration(tokens, index, Type.Int, Type.PointerInt);
                        if (declarationResultingIndex == -1) {
                            System.out.println("*** Syntax Analyzer Error ***: " +
                                    "Invalid variable declaration at index " + index + ".");
                            return null;
                        }
                        index = declarationResultingIndex;
                        }
                    break;
                case KeywordFloat:
                    {
                        final int declarationResultingIndex = processVariableDeclaration(tokens, index, Type.Float, Type.PointerFloat);
                        if (declarationResultingIndex == -1) {
                            System.out.println("*** Syntax Analyzer Error ***: " +
                                    "Invalid variable declaration at index " + index + ".");
                            return null;
                        }
                        index = declarationResultingIndex;
                    }
                    break;
                case KeywordBool:
                    {
                        final int declarationResultingIndex = processVariableDeclaration(tokens, index, Type.Bool, Type.PointerBool);
                        if (declarationResultingIndex == -1) {
                            System.out.println("*** Syntax Analyzer Error ***: " +
                                    "Invalid variable declaration at index " + index + ".");
                            return null;
                        }
                        index = declarationResultingIndex;
                    }
                    break;
                case Id:
                    {
                        final int assignmentResultingIndex = processAssignment(tokens, index);
                        if (assignmentResultingIndex == -1) {
                            System.out.println("*** Syntax Analyzer Error ***: " +
                                    "Invalid assignment at index " + index + ".");
                            return null;
                        }
                        index = assignmentResultingIndex;
                    }
                    break;
                case KeywordIf:
                    {
                        if (index + 5 >= tokens.size()) {
                            System.out.println("*** Syntax Analyzer Error ***: " +
                                    "Unexpected input end. Too few tokens for an If block at index " + index + ".");
                            return null;
                        }

                        if (tokens.get(index + 1).type != TokenType.ParenthesesOpen) {
                            System.out.println("*** Syntax Analyzer Error ***: " +
                                    "Invalid If condition: '(' expected at index " + index + ".");
                            return null;
                        }
                        index += 2;

                        final int expressionResultingIndex = processExpression(tokens, index, Type.Bool);
                        if (expressionResultingIndex == -1) {
                            System.out.println("*** Syntax Analyzer Error ***: " +
                                    "Invalid If condition at index " + index + ".");
                            return null;
                        }
                        index = expressionResultingIndex;

                        if (tokens.get(index).type != TokenType.ParenthesesClose) {
                            System.out.println("*** Syntax Analyzer Error ***: " +
                                    "Invalid If condition: ')' expected at index " + index + ".");
                            return null;
                        }
                        index++;

                        // Adding the IF Code
                        code.append("\tcmp EAX, 1\n");
                        code.append("\tjne label" + availableLabel + "\n");

                        // The IF block
                        if (tokens.get(index).type == TokenType.BracesOpen) {
                            index++; // skip {
                            while (tokens.get(index).type != TokenType.BracesClose) {
                                final int assignmentResultingIndex = processAssignment(tokens, index);
                                if (assignmentResultingIndex == -1) {
                                    System.out.println("*** Syntax Analyzer Error ***: " +
                                            "Invalid assignment at index " + index + ".");
                                    return null;
                                }
                                index = assignmentResultingIndex;
                            }

                            // Found '}'
                            index++; // skip it
                        } else {
                            final int assignmentResultingIndex = processAssignment(tokens, index);
                            if (assignmentResultingIndex == -1) {
                                System.out.println("*** Syntax Analyzer Error ***: " +
                                        "Invalid assignment at index " + index + ".");
                                return null;
                            }
                            index = assignmentResultingIndex;
                        }

                        if (index >= tokens.size()) {
                            // The IF block was the last input
                            break;
                        }

                        // End the True block
                        code.append("\tjmp label" + (availableLabel + 1) + "\n");

                        code.append("\tlabel" + availableLabel + ":\n");

                        // Now expecting either ELSE or something new
                        if (tokens.get(index).type == TokenType.KeywordElse) {
                            index++;
                            if (index + 1 >= tokens.size()) {
                                System.out.println("*** Syntax Analyzer Error ***: " +
                                        "Unexpected Else block end at index " + index + ".");
                                return null;
                            }

                            // The ELSE block. Same block as before
                            if (tokens.get(index).type == TokenType.BracesOpen) {
                                index++; // skip {
                                while (tokens.get(index).type != TokenType.BracesClose) {
                                    final int assignmentResultingIndex = processAssignment(tokens, index);
                                    if (assignmentResultingIndex == -1) {
                                        System.out.println("*** Syntax Analyzer Error ***: " +
                                                "Invalid assignment at index " + index + ".");
                                        return null;
                                    }
                                    index = assignmentResultingIndex;
                                }

                                // Found '}'
                                index++; // skip it
                            } else {
                                final int assignmentResultingIndex = processAssignment(tokens, index);
                                if (assignmentResultingIndex == -1) {
                                    System.out.println("*** Syntax Analyzer Error ***: " +
                                            "Invalid assignment at index " + index + ".");
                                    return null;
                                }
                                index = assignmentResultingIndex;
                            }
                        }

                        // Finish the Code
                        code.append("\tlabel" + (availableLabel + 1) + ":\n");

                        availableLabel += 2;

                        // Done with IF[-ELSE] statement
                    }
                    break;

                default:
                    System.out.println("*** Syntax Analyzer Error ***: " +
                            "Unexpected starting token at " + index + ": " + currentTokenType + ".");
                    return null;
            }
        }

        return code;
    }


    // call me when you expect an assignment at this place and you are sure that
    // the first token is an Id.
    // index === start index.
    // returns the new index(just after the assignment end), or -1 in case of error.
    private int processAssignment(List<Token> tokens, int index) {
        final String variableName = tokens.get(index).value;
        if (index + 3 >= tokens.size()) {
            System.out.println("*** Syntax Analyzer Error ***: " +
                    "Unexpected input end. Too few tokens for an assignment for Id " + variableName + " at index " + index + ".");
            return -1;
        }

        final Token operatorToken = tokens.get(index + 1);
        if (operatorToken.type != TokenType.OperatorAssign) {
            System.out.println("*** Syntax Analyzer Error ***: " +
                    "Expected '=' after Id '" + variableName + "', but found " + operatorToken.type + " at index " + index + " instead.");
            return -1;
        }
        index += 2;

        final Type variableType = getVariableType(variableName);
        if (variableType == null) {
            System.out.println("*** Syntax Analyzer Error ***: " +
                    "Assignment to an undeclared variable: " + variableName + ", at index " + index + ".");
            return -1;
        }

        final int expressionResultingIndex = processExpression(tokens, index, variableType);
        if (expressionResultingIndex == -1) {
            System.out.println("*** Syntax Analyzer Error ***: " +
                    "Invalid expression at index " + index + ".");
            return -1;
        }
        index = expressionResultingIndex;

        final Token lastToken = tokens.get(index);
        if (index >= tokens.size() || lastToken.type != TokenType.Semicolon) {
            System.out.println("*** Syntax Analyzer Error ***: " +
                    "Expected ';' at index " + index + ", but found " + lastToken.type + " instead.");
            return -1;
        }
        index++;

        // Add the appropriate Code
        code.append("\tmov " + variableName + ", EAX\n");

        return index;
    }


    // call me when you expect an expression at this place.
    // index === start index.
    // returns the new index(just after the expression end), or -1 in case of error.
    // TODO: add code to Codes
    private int processExpression(List<Token> tokens, int index, Type expectedType) {
        int parenthesesLeftToClose = 0;
        boolean endFound = false;
        int expressionEndIndex;
        for (expressionEndIndex = index; expressionEndIndex < tokens.size(); expressionEndIndex++) {
            final TokenType tokenType = tokens.get(expressionEndIndex).type;
            switch (tokenType) {
                case Id:
                case LiteralInt:
                case LiteralFloat:
                case LiteralBool:
                case OperatorPlus:
                case OperatorMinus:
                case OperatorMultiply:
                case OperatorDivide:
                case OperatorEquals:
                case OperatorNotEquals:
                case OperatorLess:
                case OperatorLessOrEquals:
                case OperatorGreater:
                case OperatorGreaterOrEquals:
                    continue;
                case ParenthesesOpen:
                    parenthesesLeftToClose++;
                    continue;
                case ParenthesesClose:
                    if (parenthesesLeftToClose == 0) {
                        endFound = true;
                    } else {
                        parenthesesLeftToClose--;
                    }
                    break;

                default:
                    endFound = true;
                    break;
            }

            if (endFound) {
                break;
            }
        }

        if (!endFound) {
            System.out.println("*** Syntax Analyzer Error ***: " +
                    "Could not find proper expression end.");
            return -1;
        }

        // convert
        final List<Part> parts = convertIntoParts(tokens, index, expressionEndIndex);
        if (parts == null) {
            // The error is already printed in the convertIntoParts function.
            return -1;
        }

        final PartType lastPartType = parts.get(parts.size() - 1).partType;
        if (lastPartType != PartType.Expression && lastPartType != PartType.ParenthesesClose) {
            System.out.println("*** Syntax Analyzer Error ***: " +
                    "Invalid expression: Invalid last token. Expected ')' or expression.");
            return -1;
        }

        if (!checkExpressionValidity(parts)) {
            // The error is already printed in the checkExpressionValidity function.
            return -1;
        }

        final Part part = reduceIntoPart(parts);
        if (part == null) {
            // The error is already printed in the checkExpressionValidity function.
            return -1;
        }

        if (part.type != expectedType && (!(part.type == Type.Int && expectedType == Type.Float))) {
            System.out.println("*** Syntax Analyzer Error ***: " +
                    "The type on the right of the '=' does not match the type on the left side. At " + index + ".");
            return -1;
        }

        // Add the appropriate Code
        code.append(part.code);

        return expressionEndIndex;
    }


    private Part reduceIntoPart(List<Part> parts) {
        boolean somethingChanged;
        do {
            // System.out.println("------Now:");
            // for (Part part : parts) {
            //     System.out.print(part.partType + " ");
            // }
            // System.out.println();
            somethingChanged = false;

            // ( EXPR )
            final int smallParenthesesPatternIndex = findSmallParenthesesPattern(parts);
            if (smallParenthesesPatternIndex != -1) {
                parts.remove(smallParenthesesPatternIndex + 2);
                parts.remove(smallParenthesesPatternIndex);

                somethingChanged = true;
                continue;
            }

            // EXPR OP EXPR
            final int operatorApplicationIndex = findOperatorApplication(parts);
            if (operatorApplicationIndex == -1) {
                continue;
            }

            final Part operator = parts.get(operatorApplicationIndex); // PartType.Operator..
            final Part left = parts.get(operatorApplicationIndex - 1); // PartType.Expression
            final Part right = parts.get(operatorApplicationIndex + 1); // PartType.Expression
            final Type leftType = left.type; // Int, PointerInt, ...
            final Type rightType = right.type; // Int, PointerInt, ...

            Part newPart = null;
            final String newCode =
                constructCode(left.code, operator.code, right.code);
            boolean typesWereViolated = false;
            switch (operator.partType) {
                case OperatorPlus:
                case OperatorMinus:
                    if ((leftType == Type.Float || leftType == Type.Int) &&
                            (leftType == rightType)) {
                        newPart = new Part(PartType.Expression, leftType, newCode);
                    } else if (leftType == Type.Float || rightType == Type.Float) {
                        newPart = new Part(PartType.Expression, Type.Float, newCode);
                    } else if (isTypePointer(leftType) && rightType == Type.Int) {
                        newPart = new Part(PartType.Expression, leftType, newCode);
                    } else if (isTypePointer(rightType) && leftType == Type.Int
                            && operator.partType == PartType.OperatorPlus) {
                        newPart = new Part(PartType.Expression, rightType, newCode);
                    } else {
                        typesWereViolated = true;
                    }
                    break;
                case OperatorMultiply:
                case OperatorDivide:
                    if ((leftType == Type.Float || leftType == Type.Int) &&
                            (leftType == rightType)) {
                        newPart = new Part(PartType.Expression, leftType, newCode);
                    } else if ((leftType == Type.Float && rightType == Type.Int) || (rightType == Type.Float && leftType == Type.Int)) {
                        newPart = new Part(PartType.Expression, Type.Float, newCode);
                    } else {
                        typesWereViolated = true;
                    }
                    break;
                case OperatorLess:
                case OperatorLessOrEquals:
                case OperatorGreater:
                case OperatorGreaterOrEquals:
                    if ((leftType == Type.Float || leftType == Type.Int) &&
                            (rightType == Type.Float || rightType == Type.Int)) {
                        newPart = new Part(PartType.Expression, Type.Bool, newCode);
                    } else {
                        typesWereViolated = true;
                    }
                    break;
                case OperatorEquals:
                case OperatorNotEquals:
                    if (leftType == rightType) {
                        newPart = new Part(PartType.Expression, Type.Bool, newCode);
                    } else {
                        typesWereViolated = true;
                    }
                    break;
                default:
                    System.out.println("*** Syntax Analyzer Error ***: " +
                        "Internal unknown error.");
                    return null;
            }

            if (typesWereViolated) {
                System.out.println("*** Syntax Analyzer Error ***: " +
                    "Incompatible types with operator: " + left.type + " " +
                    operator.partType + " " + right.type + ".");
                return null;
            }

            // Replace EXPR OP EXPR with NEW_EXPR
            parts.remove(operatorApplicationIndex + 1);
            parts.remove(operatorApplicationIndex + 0);
            parts.remove(operatorApplicationIndex - 1);
            parts.add(operatorApplicationIndex - 1, newPart);
            somethingChanged = true;

        } while (somethingChanged);

        if (parts.size() > 1) {
            System.out.println("^^^^^" + "more than 1");
            // Try to find the error

            // final int operatorApplicationIndex = findOperatorApplication(parts);
            // if (operatorApplicationIndex != -1) {
            //     final Part left = parts.get(operatorApplicationIndex - 1);
            //     final Part right = parts.get(operatorApplicationIndex + 1);
            //     // it did not get reduced => could not => incompatible types
            //     System.out.println("*** Syntax Analyzer Error ***: " +
            //             "Incompatible types: " + left.type + " and " + right.type + ".");
            // }

            // Hmmm... What other kind of error could it possibly be??
            // Let's hope none

            return null;
        }

        return parts.get(0);
    }


    private boolean isTypePointer(Type type) {
        return (type == Type.PointerInt || type == Type.PointerFloat || type == Type.PointerBool);
    }


    private String constructCode(String leftCode, String operatorCode, String rightCode) {
        return
            rightCode +
            "\tpush EAX\n" +
            leftCode +
            "\tpop EBX\n" +
            operatorCode;
    }


    // To be called right after the convertion into parts, so that
    // all expressions are for sure Literals or Variables.
    // Does not cover 100% of cases
    private boolean checkExpressionValidity(List<Part> parts) {
        boolean lastWasExpression = false;
        boolean lastWasOperator = false;
        boolean lastWasParenthesesOpen = true;
        for (int i = 0; i < parts.size(); i++) {
            final PartType partType = parts.get(i).partType;
            if (lastWasExpression) {
                if (partType == PartType.Expression) {
                    System.out.println("*** Syntax Analyzer Error ***: " +
                            "Invalid expression: Expression after Expression. Operator Expected.");
                    return false;
                } else if (partType == PartType.ParenthesesOpen) {
                    System.out.println("*** Syntax Analyzer Error ***: " +
                            "Invalid expression: '(' after an Expression.");
                    return false;
                } else {
                    lastWasExpression = false;
                    lastWasOperator = isPartTypeOperator(partType);
                    lastWasParenthesesOpen = false;
                }
            } else {
                if (lastWasOperator) {
                    final boolean parenthesesOpen = partType == PartType.ParenthesesOpen;
                    if (partType == PartType.Expression || parenthesesOpen) {
                        lastWasOperator = false;
                        lastWasExpression = !parenthesesOpen;
                        lastWasParenthesesOpen = parenthesesOpen;
                    } else {
                        System.out.println("*** Syntax Analyzer Error ***: " +
                                "Invalid expression: Invalid token after operator.");
                        return false;
                    }
                } else { // parentheses
                    if (lastWasParenthesesOpen) {
                        if (partType == PartType.Expression) {
                            lastWasParenthesesOpen = false;
                            lastWasExpression = true;
                            lastWasOperator = false;
                        } else if (partType == PartType.ParenthesesOpen) {
                            // nothing changes
                        } else {
                            System.out.println("*** Syntax Analyzer Error ***: " +
                                    "Invalid expression: Invalid token after '('. Expected '(' or expression.");
                            return false;
                        }
                    } else { // ')'
                        if (partType == PartType.ParenthesesClose) {
                            // nothing changes
                        } else if (isPartTypeOperator(partType)) {
                            lastWasOperator = true;
                            lastWasExpression = false;
                            lastWasParenthesesOpen = false;
                        } else {
                            System.out.println("*** Syntax Analyzer Error ***: " +
                                    "Invalid expression: Invalid token after ')'. Expected ')' or operator.");
                            return false;
                        }
                    }
                }
            }
        }

        return true;
    }

    private boolean isPartTypeParentheses(PartType partType) {
        if (partType == PartType.ParenthesesOpen ||
            partType == PartType.ParenthesesClose) {
            return true;
        }

        return false;
    }

    private boolean isPartTypeOperator(PartType partType) {
        if (partType == PartType.ParenthesesOpen ||
            partType == PartType.ParenthesesClose ||
            partType == PartType.Expression) {
            return false;
        }

        return true;
    }


    private int findOperatorApplication(List<Part> parts) {
        for (int i = 1; i < parts.size() - 1; i++) {
            final Part operator = parts.get(i);
            final PartType operatorPartType = operator.partType;
            if (operatorPartType == PartType.ParenthesesOpen ||
                operatorPartType == PartType.ParenthesesClose ||
                operatorPartType == PartType.Expression) {
                continue;
            }

            // Definetely some kind of operator

            final Part left = parts.get(i - 1);
            final Part right = parts.get(i + 1);
            if (left.partType == PartType.Expression && right.partType == PartType.Expression) {
                return i;
            }
        }

        return -1;
    }


    // returns the position of the pattern, -1 otherwise
    private int findSmallParenthesesPattern(List<Part> parts) {
        for (int i = 0; i < parts.size() - 2; i++) {
            final PartType part1 = parts.get(i).partType;
            final PartType part2 = parts.get(i + 1).partType;
            final PartType part3 = parts.get(i + 2).partType;

            if (part1 == PartType.ParenthesesOpen &&
                part2 == PartType.Expression &&
                part3 == PartType.ParenthesesClose) {
                return i;
            }
        }

        return -1;
    }


    // [start;end)
    private List<Part> convertIntoParts(List<Token> tokens, int start, int end) {
        List<Part> parts = new ArrayList<>();
        for (int i = start; i < end; i++) {
            final TokenType tokenType = tokens.get(i).type;
            final String tokenValue = tokens.get(i).value;
            switch (tokenType) {
                case OperatorPlus:
                    parts.add(new Part(PartType.OperatorPlus, null, "\tadd EAX, EBX\n"));
                    break;
                case OperatorMinus:
                    parts.add(new Part(PartType.OperatorMinus, null, "\tsub EAX, EBX\n"));
                    break;
                case OperatorMultiply:
                    parts.add(new Part(PartType.OperatorMultiply, null, "\tmul EBX\n"));
                    break;
                case OperatorDivide:
                    parts.add(new Part(PartType.OperatorDivide, null, "\tdiv EBX\n"));
                    break;

                case OperatorLess:
                    parts.add(new Part(PartType.OperatorLess, null,
                            "\tcmp EAX, EBX\n" +
                            "\tjl label" + availableLabel + "\n" +
                            "\tmov EAX, 0\n" +
                            "\tjmp label" + (availableLabel + 1) + "\n" +
                            "\tlabel" + availableLabel++ + ":\n" +
                            "\tmov EAX, 1\n" +
                            "\tlabel" + availableLabel++ + ":\n" // +1'ed
                    ));
                    break;
                case OperatorLessOrEquals:
                    parts.add(new Part(PartType.OperatorLessOrEquals, null,
                            "\tcmp EAX, EBX\n" +
                            "\tjle label" + availableLabel + "\n" +
                            "\tmov EAX, 0\n" +
                            "\tjmp label" + (availableLabel + 1) + "\n" +
                            "\tlabel" + availableLabel++ + ":\n" +
                            "\tmov EAX, 1\n" +
                            "\tlabel" + availableLabel++ + ":\n" // +1'ed
                    ));
                    break;
                case OperatorGreater:
                    parts.add(new Part(PartType.OperatorGreater, null,
                            "\tcmp EAX, EBX\n" +
                            "\tjg label" + availableLabel + "\n" +
                            "\tmov EAX, 0\n" +
                            "\tjmp label" + (availableLabel + 1) + "\n" +
                            "\tlabel" + availableLabel++ + ":\n" +
                            "\tmov EAX, 1\n" +
                            "\tlabel" + availableLabel++ + ":\n" // +1'ed
                    ));
                    break;
                case OperatorGreaterOrEquals:
                    parts.add(new Part(PartType.OperatorGreaterOrEquals, null,
                            "\tcmp EAX, EBX\n" +
                            "\tjge label" + availableLabel + "\n" +
                            "\tmov EAX, 0\n" +
                            "\tjmp label" + (availableLabel + 1) + "\n" +
                            "\tlabel" + availableLabel++ + ":\n" +
                            "\tmov EAX, 1\n" +
                            "\tlabel" + availableLabel++ + ":\n" // +1'ed
                    ));
                    break;
                case OperatorEquals:
                    parts.add(new Part(PartType.OperatorEquals, null,
                            "\tcmp EAX, EBX\n" +
                            "\tje label" + availableLabel + "\n" +
                            "\tmov EAX, 0\n" +
                            "\tjmp label" + (availableLabel + 1) + "\n" +
                            "\tlabel" + availableLabel++ + ":\n" +
                            "\tmov EAX, 1\n" +
                            "\tlabel" + availableLabel++ + ":\n" // +1'ed
                    ));
                    break;
                case OperatorNotEquals:
                    parts.add(new Part(PartType.OperatorNotEquals, null,
                            "\tcmp EAX, EBX\n" +
                            "\tjne label" + availableLabel + "\n" +
                            "\tmov EAX, 0\n" +
                            "\tjmp label" + (availableLabel + 1) + "\n" +
                            "\tlabel" + availableLabel++ + ":\n" +
                            "\tmov EAX, 1\n" +
                            "\tlabel" + availableLabel++ + ":\n" // +1'ed
                    ));
                    break;

                case ParenthesesOpen:
                    parts.add(new Part(PartType.ParenthesesOpen, null, ""));
                    break;
                case ParenthesesClose:
                    parts.add(new Part(PartType.ParenthesesClose, null, ""));
                    break;

                case Id:
                    final Type variableType = getVariableType(tokenValue);
                    if (variableType == null) {
                        System.out.println("*** Syntax Analyzer Error ***: " +
                                "Usage of undeclared variable at index " + i + ": " + tokenValue + ".");
                        return null;
                    }
                    parts.add(new Part(PartType.Expression, variableType, "\tmov EAX, " + tokenValue + "\n"));
                    break;
                case LiteralInt:
                    parts.add(new Part(PartType.Expression, Type.Int, "\tmov EAX, " + tokenValue + "\n"));
                    break;
                case LiteralFloat:
                    parts.add(new Part(PartType.Expression, Type.Float, "\tmov EAX, " + tokenValue + "\n"));
                    break;
                case LiteralBool:
                    parts.add(new Part(PartType.Expression, Type.Bool, "\tmov EAX, " + (tokenValue.equals("true") ? 1 : 0) + "\n"));
                    break;

                default:
                    System.out.println("*** Syntax Analyzer Error ***: " +
                            "Invalid token in expression at index " + i + ": " + tokenType + ".");
                    return null;
            }
        }

        return parts;
    }


    // call me when you expect a declaration at this place and you are sure that
    // the first token's type is a type keyword.
    // index === start index.
    // returns the new index(just after the ';'), or -1 in case of error.
    private int processVariableDeclaration(List<Token> tokens, int index, Type potentialRegularType, Type potentialPointerType) {
        final Token currentToken = tokens.get(index);
        final int leftAmount = tokens.size() - index;
        final TokenType currentTokenType = currentToken.type;
        if (leftAmount < 3) {
            System.out.println("*** Syntax Analyzer Error ***: " +
                    "Unexpected input end. Too few tokens after " + currentToken + " at index " + index + ".");
            return -1;
        }

        final Token secondToken = tokens.get(index + 1);
        final Type type;
        final Token variable;
        final boolean declaringPointer = secondToken.type == TokenType.OperatorMultiply;
        if (declaringPointer) {
            type = potentialPointerType;
            variable = tokens.get(index + 2);
        } else {
            type = potentialRegularType;
            variable = secondToken;
        }

        if (variable.type != TokenType.Id) {
            System.out.println("*** Syntax Analyzer Error ***: " +
                    "Id expected, but " + variable.type + " found instead at index " + index + ".");
            return -1;
        }

        if (registeredIds.containsKey(variable.value)) {
            System.out.println("*** Syntax Analyzer Error ***: " +
                    "Redeclaration of variable " + variable.value + " at index " + index + ".");
            return -1;
        }

        registeredIds.put(variable.value, type);
        index += declaringPointer ? 3 : 2;

        while (tokens.get(index).type == TokenType.Comma) {
            if (index + 2 >= tokens.size()) {
                System.out.println("*** Syntax Analyzer Error ***: " +
                        "Unexpected input end. Too few tokens after ',' after index " + index + ".");
                return -1;
            }

            final Token nextVariable = tokens.get(index + 1);
            if (nextVariable.type != TokenType.Id) {
                System.out.println("*** Syntax Analyzer Error ***: " +
                        "Expected an Id, but found " + nextVariable.type + " at " + (index + 1) + " instead.");
                return -1;
            }

            registeredIds.put(nextVariable.value, type);
            index += 2;
        }

        final Token lastToken = tokens.get(index);
        if (lastToken.type != TokenType.Semicolon) {
            System.out.println("*** Syntax Analyzer Error ***: " +
                    "Expected ';' or ',', but found " + lastToken.type + " at " + index + " instead.");
            return -1;
        }
        index++;

        return index;
    }


    private boolean areParenthesesCorrect(List<Token> tokens, int start, int end) {
        final int parenthesesCode = 0; // ()
        final int bracesCode = 1; // {}
        final int bracketsCode = 2; // []

        int[] stack = new int[tokens.size()];
        int stackTop = -1;

        for (int i = start; i < end; i++) {
            final Token token = tokens.get(i);
            final TokenType tokenType = token.type;
            switch (tokenType) {
                case ParenthesesOpen:
                    stack[++stackTop] = parenthesesCode;
                    break;
                case ParenthesesClose:
                    if (stackTop < 0 || stack[stackTop] != parenthesesCode) {
                        System.out.println("*** Syntax Analyzer Error ***: " + "Unmatched ')' parentheses at index " + i + ".");
                        return false;
                    }
                    stackTop--;
                    break;
                case BracketsOpen:
                    stack[++stackTop] = bracketsCode;
                    break;
                case BracketsClose:
                    if (stackTop < 0 || stack[stackTop] != bracketsCode) {
                        System.out.println("*** Syntax Analyzer Error ***: " + "Unmatched ']' bracket.");
                        return false;
                    }
                    stackTop--;
                    break;
                case BracesOpen:
                    stack[++stackTop] = bracesCode;
                    break;
                case BracesClose:
                    if (stackTop < 0 || stack[stackTop] != bracesCode) {
                        System.out.println("*** Syntax Analyzer Error ***: " + "Unmatched '}' braces.");
                        return false;
                    }
                    stackTop--;
                    break;
                default:
                    break;
            }
        }

        final boolean correct = stackTop == -1;
        if (!correct) {
            switch (stack[0]) {
                case parenthesesCode:
                    System.out.println("*** Syntax Analyzer Error ***: " + "Unmatched '(' parentheses.");
                    break;
                case bracketsCode:
                    System.out.println("*** Syntax Analyzer Error ***: " + "Unmatched '[' brackets.");
                    break;
                case bracesCode:
                    System.out.println("*** Syntax Analyzer Error ***: " + "Unmatched '{' braces.");
                    break;
                default:
                    break;
            }
        }

        return correct;
    }


    private Type getVariableType(String name) {
        return registeredIds.get(name);
    }


    public Map<String, Type> getRegisteredIds() {
        return registeredIds;
    }
}

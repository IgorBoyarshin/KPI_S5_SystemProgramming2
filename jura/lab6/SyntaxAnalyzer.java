import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class SyntaxAnalyzer {


    private int parsePart(List<Lexeme> lexemes, int index) {
        final Lexeme currentLexeme = lexemes.get(index);
        final LexemeType currentLexemeType = currentLexeme.type;
        switch (currentLexemeType) {
            case KEYWORD_INTEGER:
                {
                    final int declarationResultingIndex = processVariableDeclaration(lexemes, index, Type.INTEGER, Type.POINTER_INTEGER);
                    if (declarationResultingIndex == -1) {
                        System.out.println(" Syntax Analyzer Error : " +
                                "Invalid variable declaration at index " + index + ".");
                        return -1;
                    }
                    index = declarationResultingIndex;
                    }
                break;
            case KEYWORD_REAL:
                {
                    final int declarationResultingIndex = processVariableDeclaration(lexemes, index, Type.REAL, Type.POINTER_REAL);
                    if (declarationResultingIndex == -1) {
                        System.out.println(" Syntax Analyzer Error : " +
                                "Invalid variable declaration at index " + index + ".");
                        return -1;
                    }
                    index = declarationResultingIndex;
                }
                break;
            case KEYWORD_BOOLEAN:
                {
                    final int declarationResultingIndex = processVariableDeclaration(lexemes, index, Type.BOOLEAN, Type.POINTER_BOOLEAN);
                    if (declarationResultingIndex == -1) {
                        System.out.println(" Syntax Analyzer Error : " +
                                "Invalid variable declaration at index " + index + ".");
                        return -1;
                    }
                    index = declarationResultingIndex;
                }
                break;
            case ID:
                {
                    final int assignmentResultingIndex = processAssignment(lexemes, index, true);
                    if (assignmentResultingIndex == -1) {
                        System.out.println(" Syntax Analyzer Error : " +
                                "Invalid assignment at index " + index + ".");
                        return -1;
                    }
                    index = assignmentResultingIndex;
                }
                break;
            case KEYWORD_FOR:
                {
                    index++;
                    final int assignmentResultingIndex = processAssignment(lexemes, index, false);
                    if (assignmentResultingIndex == -1) {
                        System.out.println(" Syntax Analyzer Error : " +
                                "Invalid loop iterator assignment at index " + index + ".");
                        return -1;
                    }
                    index = assignmentResultingIndex;

                    if (lexemes.get(index).type != LexemeType.KEYWORD_TO) {
                        System.out.println(" Syntax Analyzer Error : " +
                                "Expected 'to', but found " + lexemes.get(index).type + " instead at index " + index + ".");
                        return -1;
                    }
                    index++;

                    final int boundResultingIndex = processExpression(lexemes, index, Type.INTEGER);
                    if (boundResultingIndex == -1) {
                        System.out.println(" Syntax Analyzer Error : " +
                                "Expected Expression after 'to', but found " + lexemes.get(index).type + " instead at index " + index + ".");
                        return -1;
                    }
                    index = boundResultingIndex;

                    if (lexemes.get(index).type != LexemeType.KEYWORD_DO) {
                        System.out.println(" Syntax Analyzer Error : " +
                                "Expected DO in loop, but found " + lexemes.get(index).type + " instead at index " + index + ".");
                        return -1;
                    }
                    index++;

                    if (lexemes.get(index).type == LexemeType.KEYWORD_BEGIN) {
                        index++; // skip 'begin'
                        while (lexemes.get(index).type != LexemeType.KEYWORD_END) {
                            final int innerResultingIndex = parsePart(lexemes, index);
                            if (innerResultingIndex == -1) {
                                // System.out.println(" Syntax Analyzer Error : " +
                                //         "Invalid assignment at index " + index + ".");
                                return -1;
                            }
                            index = innerResultingIndex;
                        }

                        // Found 'end'
                        index++; // skip it
                    } else {
                        final int innerResultingIndex = parsePart(lexemes, index);
                        if (innerResultingIndex == -1) {
                            return -1;
                        }
                        index = innerResultingIndex;
                    }

                    if (index >= lexemes.size()) {
                        break;
                    }

                    if (lexemes.get(index).type == LexemeType.SEMICOLON) {
                        index++;
                        break;
                    }

                }
                break;
            case KEYWORD_IF:
                {
                    if (index + 7 >= lexemes.size()) {
                        System.out.println(" Syntax Analyzer Error : " +
                                "Unexpected input end. Too few lexemes for an If block at index " + index + ".");
                        return -1;
                    }
                    index++;

                    final int expressionResultingIndex = processExpression(lexemes, index, Type.BOOLEAN);
                    if (expressionResultingIndex == -1) {
                        System.out.println(" Syntax Analyzer Error : " +
                                "Invalid If condition at index " + index + ".");
                        return -1;
                    }
                    index = expressionResultingIndex;

                    if (lexemes.get(index).type != LexemeType.KEYWORD_THEN) {
                        System.out.println(" Syntax Analyzer Error : " +
                                "Invalid If condition: 'then' expected at index " + index + ".");
                        return -1;
                    }
                    index++;

                    // The IF block
                    if (lexemes.get(index).type == LexemeType.KEYWORD_BEGIN) {
                        index++; // skip 'begin'
                        while (lexemes.get(index).type != LexemeType.KEYWORD_END) {
                            final int assignmentResultingIndex = processAssignment(lexemes, index, true);
                            if (assignmentResultingIndex == -1) {
                                System.out.println(" Syntax Analyzer Error : " +
                                        "Invalid assignment at index " + index + ".");
                                return -1;
                            }
                            index = assignmentResultingIndex;
                        }

                        // Found 'end'
                        index++; // skip it
                    } else {
                        final int assignmentResultingIndex = processAssignment(lexemes, index, true);
                        if (assignmentResultingIndex == -1) {
                            System.out.println(" Syntax Analyzer Error : " +
                                    "Invalid assignment at index " + index + ".");
                            return -1;
                        }
                        index = assignmentResultingIndex;
                    }

                    if (index >= lexemes.size()) {
                        // The IF block was the last input
                        break;
                    }

                    // Just skip ';' and disallow further 'else'
                    if (lexemes.get(index).type == LexemeType.SEMICOLON) {
                        index++;
                        break;
                    }

                    // Now expecting either ELSE or something new
                    if (lexemes.get(index).type == LexemeType.KEYWORD_ELSE) {
                        index++;
                        if (index + 1 >= lexemes.size()) {
                            System.out.println(" Syntax Analyzer Error : " +
                                    "Unexpected Else block end at index " + index + ".");
                            return -1;
                        }

                        // The ELSE block. Same block as before
                        if (lexemes.get(index).type == LexemeType.KEYWORD_BEGIN) {
                            index++; // skip 'begin'
                            while (lexemes.get(index).type != LexemeType.KEYWORD_END) {
                                final int assignmentResultingIndex = processAssignment(lexemes, index, true);
                                if (assignmentResultingIndex == -1) {
                                    System.out.println(" Syntax Analyzer Error : " +
                                            "Invalid assignment at index " + index + ".");
                                    return -1;
                                }
                                index = assignmentResultingIndex;
                            }

                            // Found 'end'
                            index++; // skip it
                        } else {
                            final int assignmentResultingIndex = processAssignment(lexemes, index, true);
                            if (assignmentResultingIndex == -1) {
                                System.out.println(" Syntax Analyzer Error : " +
                                        "Invalid assignment at index " + index + ".");
                                return -1;
                            }
                            index = assignmentResultingIndex;
                        }

                        // Just skip ';'
                        if (index < lexemes.size() && lexemes.get(index).type == LexemeType.SEMICOLON) {
                            index++;
                        }
                    }
                }
                break;

            default:
                System.out.println(" Syntax Analyzer Error : " +
                        "Unexpected starting lexeme at " + index + ": " + currentLexemeType + ".");
                return -1;
        }


        return index;
    }

    public boolean parse(List<Lexeme> lexemes) {
        if (!areBlocksCorrect(lexemes, 0, lexemes.size())) {
            return false;
        }

        int index = 0;
        while (index < lexemes.size()) {
            final int newIndex = parsePart(lexemes, index);
            if (newIndex == -1) {
                return false;
            }
            index = newIndex;
        }

        return true;
    }


    private int processAssignment(List<Lexeme> lexemes, int index, boolean wantSemicolon) {
        final String variableName = lexemes.get(index).value;
        if (index + 3 >= lexemes.size()) {
            System.out.println(" Syntax Analyzer Error : " +
                    "Unexpected input end. Too few lexemes for an assignment for Id " + variableName + " at index " + index + ".");
            return -1;
        }

        final Lexeme operatorLexeme = lexemes.get(index + 1);
        if (operatorLexeme.type != LexemeType.OPERATOR_ASSIGN) {
            System.out.println(" Syntax Analyzer Error : " +
                    "Expected '=' after Id '" + variableName + "', but found " + operatorLexeme.type + " at index " + index + " instead.");
            return -1;
        }
        index += 2;

        final Type variableType = getVariableType(variableName);
        if (variableType == null) {
            System.out.println(" Syntax Analyzer Error : " +
                    "Assignment to an undeclared variable: " + variableName + ", at index " + index + ".");
            return -1;
        }

        final int expressionResultingIndex = processExpression(lexemes, index, variableType);
        if (expressionResultingIndex == -1) {
            System.out.println(" Syntax Analyzer Error : " +
                    "Invalid expression at index " + index + ".");
            return -1;
        }
        index = expressionResultingIndex;

        final Lexeme lastLexeme = lexemes.get(index);
        if (wantSemicolon) {
            if (index >= lexemes.size() || lastLexeme.type != LexemeType.SEMICOLON) {
                System.out.println(" Syntax Analyzer Error : " +
                        "Expected ';' at index " + index + ", but found " + lastLexeme.type + " instead.");
                return -1;
            }
            index++;
        } else {
            if (index >= lexemes.size() || lastLexeme.type != LexemeType.KEYWORD_TO) {
                System.out.println(" Syntax Analyzer Error : " +
                        "Expected 'to' at index " + index + ", but found " + lastLexeme.type + " instead.");
                return -1;
            }
        }

        return index;
    }

    private int processExpression(List<Lexeme> lexemes, int index, Type expectedType) {
        int parenthesesLeftToClose = 0;
        boolean endFound = false;
        int expressionEndIndex;
        for (expressionEndIndex = index; expressionEndIndex < lexemes.size(); expressionEndIndex++) {
            final LexemeType lexemeType = lexemes.get(expressionEndIndex).type;
            switch (lexemeType) {
                case ID:
                case LITERAL_INTEGER:
                case LITERAL_REAL:
                case LITERAL_BOOLEAN:
                case OPERATOR_PLUS:
                case OPERATOR_MINUS:
                case OPERATOR_MULTIPLY:
                case OPERATOR_DIVIDE:
                case OPERATOR_EQUALS:
                case OPERATOR_NOT_EQUALS:
                case OPERATOR_LESS:
                case OPERATOR_LESS_OR_EQUALS:
                case OPERATOR_GREATER:
                case OPERATOR_GREATER_OR_EQUALS:
                case KEYWORD_SIN:
                    continue;
                case PARENTHESES_OPEN:
                    parenthesesLeftToClose++;
                    continue;
                case PARENTHESES_CLOSE:
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
            System.out.println(" Syntax Analyzer Error : " +
                    "Could not find proper expression end.");
            return -1;
        }

        // convert
        final List<Piece> pieces = convertIntoPieces(lexemes, index, expressionEndIndex);
        if (pieces == null) {
            // The error is already printed in the convertIntoPieces function.
            return -1;
        }

        final PieceType lastPieceType = pieces.get(pieces.size() - 1).pieceType;
        if (lastPieceType != PieceType.EXPRESSION && lastPieceType != PieceType.PARENTHESES_CLOSE) {
            System.out.println(" Syntax Analyzer Error : " +
                    "Invalid expression: Invalid last lexeme. Expected ')' or expression.");
            return -1;
        }

        if (!checkExpressionValidity(pieces)) {
            // The error is already printed in the checkExpressionValidity function.
            return -1;
        }

        final Piece piece = reduceIntoPiece(pieces);
        if (piece == null) {
            // The error is already printed in the checkExpressionValidity function.
            return -1;
        }

        if (piece.type != expectedType && (!(piece.type == Type.INTEGER && expectedType == Type.REAL))) {
            System.out.println(" Syntax Analyzer Error : " +
                    "The type on the right of the '=' does not match the type on the left side. At " + index + ".");
            return -1;
        }

        return expressionEndIndex;
    }


    private Piece reduceIntoPiece(List<Piece> pieces) {
        boolean somethingChanged;
        do {
            somethingChanged = false;

            // ( EXPR )
            final int smallParenthesesPatternIndex = findSmallParenthesesPattern(pieces);
            if (smallParenthesesPatternIndex != -1) {
                pieces.remove(smallParenthesesPatternIndex + 2);
                pieces.remove(smallParenthesesPatternIndex);

                somethingChanged = true;
                continue;
            }

            // SIN EXPR
            final int sinPatternIndex = findSinPattern(pieces);
            if (sinPatternIndex != -1) {
                pieces.remove(sinPatternIndex + 1);
                pieces.set(sinPatternIndex, new Piece(PieceType.EXPRESSION, Type.REAL));

                somethingChanged = true;
                continue;
            }

            // EXPR OP EXPR
            final int operatorApplicationIndex = findOperatorApplication(pieces);
            if (operatorApplicationIndex == -1) {
                continue;
            }

            final Piece operator = pieces.get(operatorApplicationIndex); // PieceType.Operator..
            final Piece left = pieces.get(operatorApplicationIndex - 1); // PieceType.Expression
            final Piece right = pieces.get(operatorApplicationIndex + 1); // PieceType.Expression
            final Type leftType = left.type; // Int, PointerInt, ...
            final Type rightType = right.type; // Int, PointerInt, ...

            Piece newPiece = null;
            boolean typesWereViolated = false;
            switch (operator.pieceType) {
                case OPERATOR_PLUS:
                case OPERATOR_MINUS:
                    if ((leftType == Type.REAL || leftType == Type.INTEGER) &&
                            (leftType == rightType)) {
                        newPiece = new Piece(PieceType.EXPRESSION, leftType);
                    } else if (leftType == Type.REAL || rightType == Type.REAL) {
                        newPiece = new Piece(PieceType.EXPRESSION, Type.REAL);
                    } else if (isTypePointer(leftType) && rightType == Type.INTEGER) {
                        newPiece = new Piece(PieceType.EXPRESSION, leftType);
                    } else if (isTypePointer(rightType) && leftType == Type.INTEGER
                            && operator.pieceType == PieceType.OPERATOR_PLUS) {
                        newPiece = new Piece(PieceType.EXPRESSION, rightType);
                    } else {
                        typesWereViolated = true;
                    }
                    break;
                case OPERATOR_MULTIPLY:
                case OPERATOR_DIVIDE:
                    if ((leftType == Type.REAL || leftType == Type.INTEGER) &&
                            (leftType == rightType)) {
                        newPiece = new Piece(PieceType.EXPRESSION, leftType);
                    } else if ((leftType == Type.REAL && rightType == Type.INTEGER) || (rightType == Type.REAL && leftType == Type.INTEGER)) {
                        newPiece = new Piece(PieceType.EXPRESSION, Type.REAL);
                    } else {
                        typesWereViolated = true;
                    }
                    break;
                case OPERATOR_LESS:
                case OPERATOR_LESS_OR_EQUALS:
                case OPERATOR_GREATER:
                case OPERATOR_GREATER_OR_EQUALS:
                    if ((leftType == Type.REAL || leftType == Type.INTEGER) &&
                            (rightType == Type.REAL || rightType == Type.INTEGER)) {
                        newPiece = new Piece(PieceType.EXPRESSION, Type.BOOLEAN);
                    } else {
                        typesWereViolated = true;
                    }
                    break;
                case OPERATOR_EQUALS:
                case OPERATOR_NOT_EQUALS:
                    if (leftType == rightType) {
                        newPiece = new Piece(PieceType.EXPRESSION, Type.BOOLEAN);
                    } else {
                        typesWereViolated = true;
                    }
                    break;
                default:
                    System.out.println(" Syntax Analyzer Error : " +
                        "Internal unknown error.");
                    return null;
            }

            if (typesWereViolated) {
                System.out.println(" Syntax Analyzer Error : " +
                    "Incompatible types with operator: " + left.type + " " +
                    operator.pieceType + " " + right.type + ".");
                return null;
            }

            // Replace EXPR OP EXPR with NEW_EXPR
            pieces.remove(operatorApplicationIndex + 1);
            pieces.remove(operatorApplicationIndex + 0);
            pieces.remove(operatorApplicationIndex - 1);
            pieces.add(operatorApplicationIndex - 1, newPiece);
            somethingChanged = true;

        } while (somethingChanged);

        if (pieces.size() > 1) {
            return null;
        }

        return pieces.get(0);
    }


    private boolean isTypePointer(Type type) {
        return (type == Type.POINTER_INTEGER || type == Type.POINTER_REAL || type == Type.POINTER_BOOLEAN);
    }


    private boolean checkExpressionValidity(List<Piece> pieces) {
        boolean lastWasExpression = false;
        boolean lastWasOperator = false;
        boolean lastWasParenthesesOpen = true;
        for (int i = 0; i < pieces.size(); i++) {
            final PieceType pieceType = pieces.get(i).pieceType;
            if (pieceType == PieceType.SIN) {
                continue;
            }
            if (lastWasExpression) {
                if (pieceType == PieceType.EXPRESSION) {
                    System.out.println(" Syntax Analyzer Error : " +
                            "Invalid expression: Expression after Expression. Operator Expected.");
                    return false;
                } else if (pieceType == PieceType.PARENTHESES_OPEN) {
                    System.out.println(" Syntax Analyzer Error : " +
                            "Invalid expression: '(' after an Expression.");
                    return false;
                } else {
                    lastWasExpression = false;
                    lastWasOperator = isPieceTypeOperator(pieceType);
                    lastWasParenthesesOpen = false;
                }
            } else {
                if (lastWasOperator) {
                    final boolean parenthesesOpen = pieceType == PieceType.PARENTHESES_OPEN;
                    if (pieceType == PieceType.EXPRESSION || parenthesesOpen) {
                        lastWasOperator = false;
                        lastWasExpression = !parenthesesOpen;
                        lastWasParenthesesOpen = parenthesesOpen;
                    } else {
                        System.out.println(" Syntax Analyzer Error : " +
                                "Invalid expression: Invalid lexeme after operator.");
                        return false;
                    }
                } else { // parentheses
                    if (lastWasParenthesesOpen) {
                        if (pieceType == PieceType.EXPRESSION) {
                            lastWasParenthesesOpen = false;
                            lastWasExpression = true;
                            lastWasOperator = false;
                        } else if (pieceType == PieceType.PARENTHESES_OPEN) {
                            // nothing changes
                        } else {
                            System.out.println(" Syntax Analyzer Error : " +
                                    "Invalid expression: Invalid lexeme after '('. Expected '(' or expression.");
                            return false;
                        }
                    } else { // ')'
                        if (pieceType == PieceType.PARENTHESES_CLOSE) {
                            // nothing changes
                        } else if (isPieceTypeOperator(pieceType)) {
                            lastWasOperator = true;
                            lastWasExpression = false;
                            lastWasParenthesesOpen = false;
                        } else {
                            System.out.println(" Syntax Analyzer Error : " +
                                    "Invalid expression: Invalid lexeme after ')'. Expected ')' or operator.");
                            return false;
                        }
                    }
                }
            }
        }

        return true;
    }

    private boolean isPieceTypeOperator(PieceType pieceType) {
        if (pieceType == PieceType.PARENTHESES_OPEN ||
            pieceType == PieceType.PARENTHESES_CLOSE ||
            pieceType == PieceType.EXPRESSION) {
            return false;
        }

        return true;
    }


    private int findOperatorApplication(List<Piece> pieces) {
        for (int i = 1; i < pieces.size() - 1; i++) {
            final Piece operator = pieces.get(i);
            final PieceType operatorPieceType = operator.pieceType;
            if (operatorPieceType == PieceType.PARENTHESES_OPEN ||
                operatorPieceType == PieceType.PARENTHESES_CLOSE ||
                operatorPieceType == PieceType.EXPRESSION) {
                continue;
            }

            // Definetely some kind of operator

            final Piece left = pieces.get(i - 1);
            final Piece right = pieces.get(i + 1);
            if (left.pieceType == PieceType.EXPRESSION && right.pieceType == PieceType.EXPRESSION) {
                return i;
            }
        }

        return -1;
    }

    // returns the position of the pattern, -1 otherwise
    private int findSinPattern(List<Piece> pieces) {
        for (int i = 0; i < pieces.size() - 1; i++) {
            final PieceType piece1 = pieces.get(i).pieceType;
            final PieceType piece2 = pieces.get(i + 1).pieceType;

            if (piece1 == PieceType.SIN &&
                piece2 == PieceType.EXPRESSION) {
                return i;
            }
        }

        return -1;
    }

    // returns the position of the pattern, -1 otherwise
    private int findSmallParenthesesPattern(List<Piece> pieces) {
        for (int i = 0; i < pieces.size() - 2; i++) {
            final PieceType piece1 = pieces.get(i).pieceType;
            final PieceType piece2 = pieces.get(i + 1).pieceType;
            final PieceType piece3 = pieces.get(i + 2).pieceType;

            if (piece1 == PieceType.PARENTHESES_OPEN &&
                piece2 == PieceType.EXPRESSION &&
                piece3 == PieceType.PARENTHESES_CLOSE) {
                return i;
            }
        }

        return -1;
    }


    // [start;end)
    private List<Piece> convertIntoPieces(List<Lexeme> lexemes, int start, int end) {
        List<Piece> pieces = new ArrayList<>();
        for (int i = start; i < end; i++) {
            final LexemeType lexemeType = lexemes.get(i).type;
            final String lexemeValue = lexemes.get(i).value;
            switch (lexemeType) {
                case OPERATOR_PLUS:
                    pieces.add(new Piece(PieceType.OPERATOR_PLUS, null));
                    break;
                case OPERATOR_MINUS:
                    pieces.add(new Piece(PieceType.OPERATOR_MINUS, null));
                    break;
                case OPERATOR_MULTIPLY:
                    pieces.add(new Piece(PieceType.OPERATOR_MULTIPLY, null));
                    break;
                case OPERATOR_DIVIDE:
                    pieces.add(new Piece(PieceType.OPERATOR_DIVIDE, null));
                    break;

                case OPERATOR_LESS:
                    pieces.add(new Piece(PieceType.OPERATOR_LESS, null));
                    break;
                case OPERATOR_LESS_OR_EQUALS:
                    pieces.add(new Piece(PieceType.OPERATOR_LESS_OR_EQUALS, null));
                    break;
                case OPERATOR_GREATER:
                    pieces.add(new Piece(PieceType.OPERATOR_GREATER, null));
                    break;
                case OPERATOR_GREATER_OR_EQUALS:
                    pieces.add(new Piece(PieceType.OPERATOR_GREATER_OR_EQUALS, null));
                    break;
                case OPERATOR_EQUALS:
                    pieces.add(new Piece(PieceType.OPERATOR_EQUALS, null));
                    break;
                case OPERATOR_NOT_EQUALS:
                    pieces.add(new Piece(PieceType.OPERATOR_NOT_EQUALS, null));
                    break;

                case PARENTHESES_OPEN:
                    pieces.add(new Piece(PieceType.PARENTHESES_OPEN, null));
                    break;
                case PARENTHESES_CLOSE:
                    pieces.add(new Piece(PieceType.PARENTHESES_CLOSE, null));
                    break;

                case KEYWORD_SIN:
                    pieces.add(new Piece(PieceType.SIN, null));
                    break;

                case ID:
                    final Type variableType = getVariableType(lexemeValue);
                    if (variableType == null) {
                        System.out.println(" Syntax Analyzer Error : " +
                                "Usage of undeclared variable at index " + i + ": " + lexemeValue + ".");
                        return null;
                    }
                    pieces.add(new Piece(PieceType.EXPRESSION, variableType));
                    break;
                case LITERAL_INTEGER:
                    pieces.add(new Piece(PieceType.EXPRESSION, Type.INTEGER));
                    break;
                case LITERAL_REAL:
                    pieces.add(new Piece(PieceType.EXPRESSION, Type.REAL));
                    break;
                case LITERAL_BOOLEAN:
                    pieces.add(new Piece(PieceType.EXPRESSION, Type.BOOLEAN));
                    break;

                default:
                    System.out.println(" Syntax Analyzer Error : " +
                            "Invalid lexeme in expression at index " + i + ": " + lexemeType + ".");
                    return null;
            }
        }

        return pieces;
    }


    private int processVariableDeclaration(List<Lexeme> lexemes, int index, Type potentialRegularType, Type potentialPointerType) {
        final Lexeme currentLexeme = lexemes.get(index);
        final int leftAmount = lexemes.size() - index;
        final LexemeType currentLexemeType = currentLexeme.type;
        if (leftAmount < 3) {
            System.out.println(" Syntax Analyzer Error : " +
                    "Unexpected input end. Too few lexemes after " + currentLexeme + " at index " + index + ".");
            return -1;
        }

        final Lexeme secondLexeme = lexemes.get(index + 1);
        final Type type;
        final Lexeme variable;
        final boolean declaringPointer = secondLexeme.type == LexemeType.POINTER;
        if (declaringPointer) {
            type = potentialPointerType;
            variable = lexemes.get(index + 2);
        } else {
            type = potentialRegularType;
            variable = secondLexeme;
        }

        if (variable.type != LexemeType.ID) {
            System.out.println(" Syntax Analyzer Error : " +
                    "Id expected, but " + variable.type + " found instead at index " + index + ".");
            return -1;
        }

        if (registeredIds.containsKey(variable.value)) {
            System.out.println(" Syntax Analyzer Error : " +
                    "Redeclaration of variable " + variable.value + " at index " + index + ".");
            return -1;
        }

        registeredIds.put(variable.value, type);
        index += declaringPointer ? 3 : 2;

        while (lexemes.get(index).type == LexemeType.COMMA) {
            if (index + 2 >= lexemes.size()) {
                System.out.println(" Syntax Analyzer Error : " +
                        "Unexpected input end. Too few lexemes after ',' after index " + index + ".");
                return -1;
            }

            final Lexeme nextVariable = lexemes.get(index + 1);
            if (nextVariable.type != LexemeType.ID) {
                System.out.println(" Syntax Analyzer Error : " +
                        "Expected an Id, but found " + nextVariable.type + " at " + (index + 1) + " instead.");
                return -1;
            }

            registeredIds.put(nextVariable.value, type);
            index += 2;
        }

        final Lexeme lastLexeme = lexemes.get(index);
        if (lastLexeme.type != LexemeType.SEMICOLON) {
            System.out.println(" Syntax Analyzer Error : " +
                    "Expected ';' or ',', but found " + lastLexeme.type + " at " + index + " instead.");
            return -1;
        }
        index++;

        return index;
    }


    private boolean areBlocksCorrect(List<Lexeme> lexemes, int start, int end) {
        final int parenthesesCode = 0; // ()
        final int bracesCode = 1; // {}
        final int bracketsCode = 2; // []
        final int blockCode = 3; // begin end

        int[] stack = new int[lexemes.size()];
        int stackTop = -1;

        for (int i = start; i < end; i++) {
            final Lexeme lexeme = lexemes.get(i);
            final LexemeType lexemeType = lexeme.type;
            switch (lexemeType) {
                case PARENTHESES_OPEN:
                    stack[++stackTop] = parenthesesCode;
                    break;
                case PARENTHESES_CLOSE:
                    if (stackTop < 0 || stack[stackTop] != parenthesesCode) {
                        System.out.println(" Syntax Analyzer Error : " + "Unmatched ')' parentheses at index " + i + ".");
                        return false;
                    }
                    stackTop--;
                    break;
                case BRACKETS_OPEN:
                    stack[++stackTop] = bracketsCode;
                    break;
                case BRACKETS_CLOSE:
                    if (stackTop < 0 || stack[stackTop] != bracketsCode) {
                        System.out.println(" Syntax Analyzer Error : " + "Unmatched ']' bracket.");
                        return false;
                    }
                    stackTop--;
                    break;
                case BRACES_OPEN:
                    stack[++stackTop] = bracesCode;
                    break;
                case BRACES_CLOSE:
                    if (stackTop < 0 || stack[stackTop] != bracesCode) {
                        System.out.println(" Syntax Analyzer Error : " + "Unmatched '}' braces.");
                        return false;
                    }
                    stackTop--;
                    break;
                case KEYWORD_BEGIN:
                    stack[++stackTop] = blockCode;
                    break;
                case KEYWORD_END:
                    if (stackTop < 0 || stack[stackTop] != blockCode) {
                        System.out.println(" Syntax Analyzer Error : " + "Unmatched 'end' keyword.");
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
                    System.out.println(" Syntax Analyzer Error : " + "Unmatched '(' parentheses.");
                    break;
                case bracketsCode:
                    System.out.println(" Syntax Analyzer Error : " + "Unmatched '[' brackets.");
                    break;
                case bracesCode:
                    System.out.println(" Syntax Analyzer Error : " + "Unmatched '{' braces.");
                    break;
                case blockCode:
                    System.out.println(" Syntax Analyzer Error : " + "Unmatched 'begin' keyword.");
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


    private Map<String, Type> registeredIds = new HashMap<>();
}

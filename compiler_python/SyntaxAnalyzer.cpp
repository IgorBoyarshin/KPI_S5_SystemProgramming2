#include "SyntaxAnalyzer.h"


SyntaxAnalyzer::SyntaxAnalyzer() :
    m_Nodes(std::vector<Node>()),
    m_VariablesTable(std::map<std::string, Type>()),
    m_NextLabelNumberToUse(0) {}


bool SyntaxAnalyzer::parse(const std::vector<Token>& tokens) {
    const std::vector<std::vector<Token>> lines = split(tokens, TokenType_NewLine);

    // "Fd" === Function Definition
    bool insideFd = false;
    std::string currentFdName;
    unsigned int currentFdAmountOfArguments = 0;

    unsigned int lineIndex = 0;
    for (const std::vector<Token>& currentLine : lines) {
        lineIndex++;

        const unsigned int shiftLevel = getShiftLevel(currentLine);
        const std::vector<Token> line = stripFromTabs(currentLine);

        if (line.size() == 0) {
            if (insideFd) {
                // Function definition has ended, create an approptiate Node
                m_Nodes.push_back(
                    NodeFunctionEnd(currentFdName, currentFdAmountOfArguments)
                );
                insideFd = false;
            }
            continue;
        }

        const TokenType firstTokenType = line.at(0).type;
        switch (firstTokenType) {
            case TokenType_KeywordDef: {
                if (insideFd || shiftLevel > 0) {
                    std::cout << "Syntax Error:"
                              << " Invalid function definition."
                              << " A new function must be declared with indentation"
                              << " 0 and within global scope."
                              << " In line " << lineIndex << "."
                              << std::endl;
                    return false;
                }
                if (line.size() < 5) {
                    std::cout << "Syntax Error:"
                              << " Invalid function definition."
                              << " Too few tokens."
                              << " In line " << lineIndex << "."
                              << std::endl;
                    return false;
                }
                insideFd = true;

                const Token tokenId = line.at(1);
                const TokenType tokenIdType = tokenId.type;
                const std::string tokenIdValue = tokenId.value;
                if (tokenIdType != TokenType_Identifier) {
                    std::cout << "Syntax Error:"
                              << " Invalid function definition."
                              << " Expected Identifier after 'def'."
                              << " In line " << lineIndex << "."
                              << std::endl;
                    return false;
                }
                if (m_VariablesTable.find(tokenIdValue) != m_VariablesTable.end()) {
                    std::cout << "Syntax Error:"
                              << " Invalid function definition."
                              << " Function redefinition."
                              << " In line " << lineIndex << "."
                              << std::endl;
                    return false;
                }
                currentFdName = tokenIdValue;

                const char firstChar = tokenIdValue[0];
                const Type functionType = (firstChar == 'b') ?
                    Type_FuncBool : Type_FuncNumber;

                m_VariablesTable.insert({tokenIdValue, functionType});

                if (line.at(2).value.compare("(") != 0) {
                    std::cout << "Syntax Error:"
                              << " Invalid function definition."
                              << " Expected '(' after function name."
                              << " In line " << lineIndex << "."
                              << std::endl;
                    return false;
                }

                unsigned int iterator;
                bool expectId = true;
                std::vector<Argument> arguments;
                for (iterator = 3; iterator < line.size() - 2; iterator++) {
                    const TokenType argumentType = line.at(iterator).type;
                    const std::string argumentValue = line.at(iterator).value;

                    if (expectId) {
                        if (argumentType != TokenType_Identifier) {
                            std::cout << "Syntax Error:"
                                      << " Invalid function definition."
                                      << " Expected Identifier as an argument,"
                                      << " but found " << argumentType << " instead."
                                      << " In line " << lineIndex << "."
                                      << std::endl;
                            return false;
                        }
                        if (m_VariablesTable.find(argumentValue) !=
                                m_VariablesTable.end()) {
                            std::cout << "Syntax Error:"
                                      << " Invalid function definition."
                                      << " Variable redefinition: "
                                      << argumentValue << "."
                                      << " In line " << lineIndex << "."
                                      << std::endl;
                            return false;
                        }

                        const char firstCharOfVariable = argumentValue[0];
                        const Type argumentType = (firstCharOfVariable == 'b') ?
                            Type_Bool : Type_Number;

                        arguments.emplace_back(argumentValue, argumentType);
                        m_VariablesTable.insert({argumentValue, argumentType});

                        expectId = false;
                    } else {
                        if (argumentValue.compare(",") == 0) {
                            expectId = true;
                        } else {
                            // We can't have reached ')', because it is supposed to
                            // have index (strippedLine.size() - 2), but the loop
                            // doesn't go up to that value. So an error for sure.
                            std::cout << "Syntax Error:"
                                      << " Invalid function definition."
                                      << " Premature end of function declaration."
                                      << " Expected ',', but found "
                                      << argumentValue << " instead."
                                      << " In line " << lineIndex << "."
                                      << std::endl;
                            return false;
                        }
                    }
                }

                if (line.at(line.size() - 1).value.compare(":") != 0) {
                    std::cout << "Syntax Error:"
                              << " Invalid function definition."
                              << " Expected ':' at the end of function declaration."
                              << " In line " << lineIndex << "."
                              << std::endl;
                    return false;
                }
                currentFdAmountOfArguments = arguments.size();

                m_Nodes.push_back(
                    NodeFunctionNew(currentFdName, arguments)
                );

                break;
            }

            case TokenType_KeywordReturn: {
                if (!insideFd) {
                    std::cout << "Syntax Error:"
                              << " Unexpected 'return' keyword: not in function."
                              << " In line " << lineIndex << "."
                              << std::endl;
                    return false;
                }

                std::stringstream ss;
                if (line.size() > 1) {
                    const Piece reducedExpression =
                        reduceExpression(line, 1, lineIndex);
                    if (reducedExpression.type == PieceType_Undefined) {
                        std::cout << "Syntax Error:"
                                  << " Invalid return expression."
                                  << " In line " << lineIndex << "."
                                  << std::endl;
                        return false;
                    }

                    ss << reducedExpression.content; // already in EAX
                }
                // Regardless of whether it was just 'return' or something
                // more complex, it's time to jump to the function end.
                ss << "\tjmp " << std::string(currentFdName) << "End\n";
                m_Nodes.push_back(
                    NodeStatement(
                        ss.str()
                    )
                );
                break;
            }
            case TokenType_Identifier: {
                const std::string idValue = line.at(0).value;
                const TokenType secondTokenType = line.at(1).type;

                if (secondTokenType == TokenType_Assign) {
                    const Type variableType = (idValue[0] == 'b') ?
                        Type_Bool : Type_Number;

                    if (m_VariablesTable.find(idValue) == m_VariablesTable.end()) {
                        m_VariablesTable.insert({idValue, variableType});
                    }

                    const Piece reducedExpression =
                        reduceExpression(line, 2, lineIndex);
                    const PieceType expressionType =
                        reducedExpression.type;
                    if (expressionType == PieceType_Undefined) {
                        std::cout << "Syntax Error:"
                                  << " Invalid expression."
                                  << " In line " << lineIndex << "."
                                  << std::endl;
                        return false;
                    }

                    const bool compatiableTypes =
                        ((variableType == Type_Bool &&
                              expressionType == PieceType_Bool) ||
                         (variableType == Type_Number &&
                            expressionType == PieceType_Number));
                    if (!compatiableTypes) {
                        std::cout << "Syntax Error:"
                                  << " Incompatible types on the left and"
                                  << " right hand side of '=':" << variableType
                                  << " and " << expressionType << "."
                                  << " In line " << lineIndex << "."
                                  << std::endl;
                        return false;
                    }

                    std::stringstream ss;
                    ss << reducedExpression.content
                       << "\tmov " << idValue << ", eax\n"
                       << "\n";

                    m_Nodes.push_back(
                        NodeStatement(ss.str())
                    );
                } else if (secondTokenType == TokenType_LeftParenthesis) {
                    // void function call

                    if (m_VariablesTable.find(idValue) == m_VariablesTable.end()) {
                        std::cout << "Syntax Error:"
                                  << " Call of undeclared function."
                                  << " In line " << lineIndex << "."
                                  << std::endl;
                        return false;
                    }

                    const Piece reducedFunctionCall =
                        reduceExpression(line, 0, lineIndex);
                    const PieceType functionType =
                        reducedFunctionCall.type;
                    if (functionType == PieceType_Undefined) {
                        std::cout << "Syntax Error:"
                                  << " Invalid function call."
                                  << " In line " << lineIndex << "."
                                  << std::endl;
                        return false;
                    }

                    m_Nodes.push_back(
                        NodeStatement(
                            reducedFunctionCall.content
                        )
                    );
                } else {
                    std::cout << "Syntax Error:"
                              << " Invalid Token after ID."
                              << " Expected '(' or '='."
                              << " In line " << lineIndex << "."
                              << std::endl;
                    return false;
                }
                break;
            }

            default:
                std::cout << "Syntax Error:"
                          << " Invalid starting Token."
                          << " In line " << lineIndex << "."
                          << std::endl;
                return false;
        }
    }
}


// Call me when you have:
// - ID_VAR = .... in which case give me the index of the first token of ....
// - ID_FUNC .... in which case give me the index of ID_FUNC(so, zero)
// - return .... in which case give me the index of the first token of ....
// If successsfull, returns one Piece containing all the code.
// Otherwise returns Piece with Undefined Type.
Piece SyntaxAnalyzer::reduceExpression(const std::vector<Token> line, unsigned int startIndex, unsigned int lineIndex) const {
    const bool parenthesesAreCorrect = areParenthesesCorrect(line);
    if (!parenthesesAreCorrect) {
        std::cout << "Syntax Error: Incorrect parentheses. "
                  << "In line " << lineIndex << "."
                  << std::endl;
        return Piece(PieceType_Undefined, ""); // error
    }

    std::vector<Piece> pieces = convertTokensIntoPieces(line, startIndex, lineIndex);
    if (pieces.size() == 0) {
        return Piece(PieceType_Undefined, ""); // error
    }

    bool somethingChanged;
    do {
        somethingChanged = false;

        {
            const int simpleParenthesesIndex = findSimpleParentheses(pieces);
            // if (simpleParenthesesIndex == -1) {
                // return Piece(PieceType_Undefined, ""); // error
            if (simpleParenthesesIndex >= 0) {
                pieces.erase(pieces.begin() + simpleParenthesesIndex + 2);
                pieces.erase(pieces.begin() + simpleParenthesesIndex + 0);
                somethingChanged = true;
                continue;
            } // else == -1 => not found => continue with other patterns
        }


        {
            const int operatorApplicationIndex = findOperator(pieces, lineIndex);
            if (operatorApplicationIndex == -2) {
                return Piece(PieceType_Undefined, ""); // error
            } else if (operatorApplicationIndex >= 0) {
                const Piece left = pieces.at(operatorApplicationIndex - 1);
                const Piece op = pieces.at(operatorApplicationIndex);
                const Piece right = pieces.at(operatorApplicationIndex + 1);

                std::stringstream ss;
                ss << right.content
                   << "\tpush eax\n"
                   << left.content
                   << "\tpop ebx\n"
                   << op.content;
                const Piece newPiece(
                    getOperatorResultType(op.type),
                    ss.str()
                );

                pieces.erase(pieces.begin() + operatorApplicationIndex + 1);
                pieces.erase(pieces.begin() + operatorApplicationIndex + 0);
                pieces[operatorApplicationIndex - 1] = newPiece;

                somethingChanged = true;
                continue;
            } // else == -1 => not found => contunie with other patterns
        }


        {
            const int commaExprFuncEndIndex = findCommaExprFuncEnd(pieces);
            if (commaExprFuncEndIndex != -1) {
                std::stringstream ss;
                ss << pieces.at(commaExprFuncEndIndex + 2).content
                   << pieces.at(commaExprFuncEndIndex + 1).content
                   << "\tpush eax\n";

                const Piece newPiece(
                    PieceType_FunctionEnd,
                    ss.str()
                );

                pieces.erase(pieces.begin() + commaExprFuncEndIndex + 2);
                pieces.erase(pieces.begin() + commaExprFuncEndIndex + 1);
                pieces[commaExprFuncEndIndex] = newPiece;
                // pieces.erase(pieces.begin() + commaExprFuncEndIndex);
                // pieces.insert(pieces.begin() + commaExprFuncEndIndex, newPiece);

                somethingChanged = true;
                continue;
            }
        }


        {
            const int exprFuncEndIndex = findExprFuncEnd(pieces);
            if (exprFuncEndIndex != -1) {
                std::stringstream ss;
                ss << pieces.at(exprFuncEndIndex + 1).content
                   << pieces.at(exprFuncEndIndex + 0).content
                   << "\tpush eax\n";

                const Piece newPiece(
                    PieceType_FunctionEnd,
                    ss.str()
                );

                pieces.erase(pieces.begin() + exprFuncEndIndex + 1);
                pieces[exprFuncEndIndex] = newPiece;
                // pieces.erase(pieces.begin() + exprFuncEndIndex);
                // pieces.insert(pieces.begin() + exprFuncEndIndex, newPiece);

                somethingChanged = true;
                continue;
            }
        }


        {
            const int funcBeginFuncEndIndex = findFuncBeginFuncEnd(pieces);
            if (funcBeginFuncEndIndex != -1) {
                const Piece first = pieces.at(funcBeginFuncEndIndex + 0);
                const Piece second = pieces.at(funcBeginFuncEndIndex + 1);

                std::stringstream ss;
                ss << second.content << first.content << "\n";

                const Piece newPiece(
                    first.type == PieceType_FunctionBool ?
                        PieceType_Bool :
                        PieceType_Number,
                    ss.str()
                );

                pieces.erase(pieces.begin() + funcBeginFuncEndIndex + 1);
                pieces[funcBeginFuncEndIndex] = newPiece;

                somethingChanged = true;
                continue;
            }
        }
    } while (somethingChanged);

    if (pieces.size() == 1) {
        return pieces.at(0);
    } else {
        std::cout << "Hmmmm......."
                  << std::endl;
        return Piece(PieceType_Undefined, ""); // error
    }
}


// -1 === not found
int SyntaxAnalyzer::findSimpleParentheses(const std::vector<Piece>& line) const {
    for (int i = 0; i < static_cast<int>(line.size()) - 2; i++) {
        const PieceType leftName = line.at(i).type;
        const PieceType exprName = line.at(i + 1).type;
        const PieceType rightName = line.at(i + 2).type;

        if (leftName == PieceType_ParenthesesOpen &&
            rightName == PieceType_ParenthesesClose) {
            if (exprName == PieceType_Number ||
                exprName == PieceType_Bool) {
                return i;
            }
        }
    }

    return -1;
}

// -1 === not found
int SyntaxAnalyzer::findCommaExprFuncEnd(const std::vector<Piece>& line) const {
    for (int i = 0; i < static_cast<int>(line.size()) - 2; i++) {
        const PieceType first = line.at(i).type;
        const PieceType second = line.at(i + 1).type;
        const PieceType third = line.at(i + 2).type;

        if (first == PieceType_Comma &&
            third == PieceType_FunctionEnd) {
            if (second == PieceType_Bool ||
                second == PieceType_Number) {
                return i;
            }
        }
    }

    return -1;
}

// -1 === not found
int SyntaxAnalyzer::findExprFuncEnd(const std::vector<Piece>& line) const {
    for (int i = 0; i < static_cast<int>(line.size()) - 1; i++) {
        const Piece first = line.at(i);
        const Piece second = line.at(i + 1);

        if (second.type == PieceType_FunctionEnd) {
            if (first.type == PieceType_Bool ||
                first.type == PieceType_Number) {
                return i;
            }
        }
    }

    return -1;
}

// -1 === not found
int SyntaxAnalyzer::findFuncBeginFuncEnd(const std::vector<Piece>& line) const {
    for (int i = 0; i < static_cast<int>(line.size()) - 1; i++) {
        const Piece first = line.at(i);
        const Piece second = line.at(i + 1);

        if (second.type == PieceType_FunctionEnd) {
            if (first.type == PieceType_FunctionBool ||
                first.type == PieceType_FunctionNumber) {
                return i;
            }
        }
    }

    return -1;
}


// returns the index in line where there is a valid operator application,
// -1 === not found
// -2 === error found
int SyntaxAnalyzer::findOperator(
        const std::vector<Piece>& line,
        unsigned int lineIndex) const {
    for (int i = 1; i < static_cast<int>(line.size()) - 1; i++) {
        const Piece piece = line.at(i);
        const PieceType leftType = line.at(i - 1).type;
        const PieceType rightType = line.at(i + 1).type;
        switch (piece.type) {
            case PieceType_Plus:
            case PieceType_Minus:
            case PieceType_Multiply:
            case PieceType_Divide:
            case PieceType_Greater:
            case PieceType_GreaterOrEquals:
            case PieceType_Less:
            case PieceType_LessOrEquals:
                {
                    if (leftType == PieceType_Number &&
                        rightType == PieceType_Number) {
                        return i;
                    }

                    switch (leftType) {
                        case PieceType_Number:
                        case PieceType_ParenthesesClose:
                        case PieceType_FunctionEnd:
                            // allowed
                            break;
                        case PieceType_Bool:
                            std::cout << "Semantic Error: "
                                      << "Invalid left-hand side type for operator "
                                      << piece.type << ": Bool. Expected Number. "
                                      << "In line " << lineIndex << "."
                                      << std::endl;
                            return -2;
                        default:
                            std::cout << "Semantic Error: "
                                      << "Invalid left-hand side token for operator "
                                      << piece.type << ": " << leftType << ". "
                                      << "In line " << lineIndex << "."
                                      << std::endl;
                            return -2;
                    }

                    switch (rightType) {
                        case PieceType_Number:
                        case PieceType_ParenthesesOpen:
                        case PieceType_FunctionNumber:
                            // allowed
                            break;
                        case PieceType_FunctionBool:
                            // Let the error be raised later, when the function reduced into Bool
                            break;
                        case PieceType_Bool:
                            std::cout << "Semantic Error: "
                                      << "Invalid right-hand side type for operator"
                                      << piece.type << ": Bool. Expected Number. "
                                      << "In line " << lineIndex << "."
                                      << std::endl;
                            return -2;
                        default:
                            std::cout << "Semantic Error: "
                                      << "Invalid right-hand side token for operator"
                                      << piece.type << ": " << rightType << ". "
                                      << "In line " << lineIndex << "."
                                      << std::endl;
                            return -2;
                    }
                }
                break;

            case PieceType_And:
            case PieceType_Or:
                {
                    if (leftType == PieceType_Bool &&
                        rightType == PieceType_Bool) {
                        return i;
                    }

                    switch (leftType) {
                        case PieceType_Bool:
                        case PieceType_ParenthesesClose:
                        case PieceType_FunctionEnd:
                            // allowed
                            break;
                        case PieceType_Number:
                            std::cout << "Semantic Error: "
                                      << "Invalid left-hand side type for operator "
                                      << piece.type << ": Number. Expected Bool. "
                                      << "In line "
                                      << lineIndex << "."
                                      << std::endl;
                            return -2;
                        default:
                            std::cout << "Semantic Error: "
                                      << "Invalid left-hand side token for operator "
                                      << piece.type << ": " << leftType << ". "
                                      << "In line " << lineIndex << "."
                                      << std::endl;
                            return -2;
                    }

                    switch (rightType) {
                        case PieceType_Bool:
                        case PieceType_ParenthesesOpen:
                        case PieceType_FunctionBool:
                            // allowed
                            break;
                        case PieceType_FunctionNumber:
                            // Let the error be raised later, when the function reduced into Bool
                            break;
                        case PieceType_Number:
                            std::cout << "Semantic Error: "
                                      << "Invalid right-hand side type for operator"
                                      << piece.type << ": Number. Expected Bool. "
                                      << "In line " << lineIndex << "."
                                      << std::endl;
                            return -2;
                        default:
                            std::cout << "Semantic Error: "
                                      << "Invalid right-hand side token for operator"
                                      << piece.type << ": " << rightType << ". "
                                      << "In line " << lineIndex << "."
                                      << std::endl;
                            return -2;
                    }
                }
                break;

            case PieceType_Equals:
                {
                    if ((leftType == PieceType_Bool && rightType == PieceType_Bool) ||
                        (leftType == PieceType_Bool && rightType == PieceType_Bool)) {
                        return i;
                    }

                    if ((leftType == PieceType_FunctionEnd ||
                            leftType == PieceType_ParenthesesClose) &&
                        (rightType == PieceType_FunctionNumber ||
                         rightType == PieceType_FunctionBool ||
                         rightType == PieceType_ParenthesesOpen)) {
                        // Allowed
                    } else {
                        std::cout << "Semantic Error: Invalid operator application: "
                                  << "<left> <op> <right>: "
                                  << leftType << " " << piece.type << " " << rightType
                                  << ". In line " << lineIndex << "."
                                  << std::endl;
                        return -2;
                    }
                }
                break;

            default:
                // keep searching..
                break;
        }
    }

    return -1;
}


std::vector<Piece> SyntaxAnalyzer::convertTokensIntoPieces(
        const std::vector<Token>& tokens,
        unsigned int startIndex,
        unsigned int lineIndex) const {
    std::vector<Piece> pieces;
    unsigned int opennedParentheses = 0;
    // Holds the value of variable opennedParentheses when a function was started.
    std::vector<unsigned int> whenFunctionsBegan;
    for (unsigned int index = startIndex; index < tokens.size(); index++) {
        const TokenType tokenType = tokens.at(index).type;
        const std::string& tokenValue = tokens.at(index).value;

        switch (tokenType) {
            case TokenType_Number:
                pieces.emplace_back(
                    PieceType_Number,
                    "\tmov eax, " + tokenValue + "\n"
                );
                break;
            case TokenType_Bool:
                pieces.emplace_back(
                    PieceType_Bool,
                    "\tmov eax, " +
                    (tokenValue.compare("True") == 0 ?
                        std::string("1") :
                        std::string("0")) +
                    "\n");
                break;
            case TokenType_Identifier: {
                const auto entry = m_VariablesTable.find(tokenValue);
                if (entry == m_VariablesTable.end()) {
                    std::cout << "Syntax Error: Usage of undeclared identifier: "
                              << tokenValue
                              << ". In line " << lineIndex << "."
                              << std::endl;
                    return std::vector<Piece>(); // empty
                }

                const Type identifierType = entry->second;
                switch (identifierType) {
                    case Type_Number:
                    case Type_Bool:
                        if (index + 1 < tokens.size() &&
                            tokens.at(index + 1).type == TokenType_LeftParenthesis) {
                            std::cout << "Syntax Error: Invalid function call: '"
                                      << tokenValue << "' is not a function."
                                      << " In line " << lineIndex << "."
                                      << std::endl;
                            return std::vector<Piece>(); // empty
                        }

                        pieces.emplace_back(
                            identifierType == Type_Bool ?
                                PieceType_Bool :
                                PieceType_Number,
                            "\tmov eax, " + tokenValue + "\n"
                        );

                        break;
                    case Type_FuncNumber:
                    case Type_FuncBool:
                        if (index + 2 < tokens.size() &&
                            tokens.at(index + 1).type == TokenType_LeftParenthesis) {
                            if (m_VariablesTable.find(tokenValue) ==
                                    m_VariablesTable.end()) {
                                std::cout << "Syntax Error: Usage of undefined function: "
                                          << tokenValue << "."
                                          << " In line " << lineIndex << "."
                                          << std::endl;
                                return std::vector<Piece>(); // empty
                            }

                            index++; // slip '('
                            whenFunctionsBegan.push_back(opennedParentheses++);
                            pieces.emplace_back(
                                identifierType == Type_FuncBool ?
                                    PieceType_FunctionBool :
                                    PieceType_FunctionNumber,
                                "\tcall " + tokenValue + "\n"
                            );
                        } else {
                            std::cout << "Syntax Error: Invalid expression: "
                                      << "Expected '(' after functional ID. "
                                      << "In line " << lineIndex << "."
                                      << std::endl;
                            return std::vector<Piece>(); // empty
                        }
                        break;
                }
                break;
            }
            case TokenType_LeftParenthesis:
                pieces.emplace_back(
                    PieceType_ParenthesesOpen,
                    ""
                );
                opennedParentheses++;
                break;
            case TokenType_RightParenthesis: {
                opennedParentheses--;
                const unsigned int size = whenFunctionsBegan.size();
                if (size > 0 &&
                    whenFunctionsBegan.at(size - 1) == opennedParentheses) {
                    // Current parenthesis matches the one that started a function
                    whenFunctionsBegan.pop_back();
                    pieces.emplace_back(
                        PieceType_FunctionEnd,
                        ""
                    );
                } else {
                    pieces.emplace_back(
                        PieceType_ParenthesesClose,
                        ""
                    );
                }

                break;
            }
            case TokenType_Comma:
                pieces.emplace_back(
                    PieceType_Comma,
                    ""
                );
                break;

            case TokenType_Plus:
                pieces.emplace_back(
                    PieceType_Plus,
                    "\tadd eax, ebx\n"
                );
                break;
            case TokenType_Minus:
                pieces.emplace_back(
                    PieceType_Minus,
                    "\tsub eax, ebx\n"
                );
                break;
            case TokenType_Multiply:
                pieces.emplace_back(
                    PieceType_Multiply,
                    "\tmul eax, ebx\n"
                );
                break;
            case TokenType_Divide:
                pieces.emplace_back(
                    PieceType_Divide,
                    "\tdiv eax, ebx\n"
                );
                break;

            case TokenType_Equals:
                pieces.emplace_back(
                    PieceType_Equals,
                    generateLogicalOperatorCode("cmp", "je")
                );
                break;
            case TokenType_Greater:
                pieces.emplace_back(
                    PieceType_Greater,
                    generateLogicalOperatorCode("cmp", "jg")
                );
                break;
            case TokenType_GreaterOrEquals:
                pieces.emplace_back(
                    PieceType_GreaterOrEquals,
                    generateLogicalOperatorCode("cmp", "jge")
                );
                break;
            case TokenType_Less:
                pieces.emplace_back(
                    PieceType_Less,
                    generateLogicalOperatorCode("cmp", "jl")
                );
                break;
            case TokenType_LessOrEquals:
                pieces.emplace_back(
                    PieceType_LessOrEquals,
                    generateLogicalOperatorCode("cmp", "jle")
                );
                break;
            case TokenType_KeywordOr:
                pieces.emplace_back(
                    PieceType_Or,
                    generateLogicalOperatorCode("or", "jnz")
                );
                break;
            case TokenType_KeywordAnd:
                pieces.emplace_back(
                    PieceType_And,
                    generateLogicalOperatorCode("and", "jnz")
                );
                break;

            case TokenType_Colon: // will be used for 'while' syntax
            // case Semicolon: // will be used in 'range' syntax
                return pieces; // signal for end of expression

            default:
                std::cout << "Syntax Error: Invalid token in expression: "
                          << tokenType << ". In line " << lineIndex << "."
                          << std::endl;
                return std::vector<Piece>(); // empty
        }
    }

    return pieces;
}


std::string SyntaxAnalyzer::generateLogicalOperatorCode(
        const std::string& operator1Name,
        const std::string& operator2Name) const {
    std::stringstream ss;

    ss << "\t" << operator1Name << " eax, ebx\n"
        << "\t" << operator2Name << " loop" << m_NextLabelNumberToUse << "\n"
        << "\tmov eax, 0\n"
        << "\tjmp loop" << (m_NextLabelNumberToUse + 1) << "\n"
        << "\tloop" << m_NextLabelNumberToUse << ":\n"
        << "\tmov eax, 1\n"
        << "\tloop" << (m_NextLabelNumberToUse + 1) << ":\n";

    m_NextLabelNumberToUse++;
    m_NextLabelNumberToUse++;

    return ss.str();
}


std::vector<std::vector<Token>> SyntaxAnalyzer::split(
        const std::vector<Token>& tokens,
        TokenType separator) const {
    std::vector<std::vector<Token>> lines;
    const unsigned int size = tokens.size();
    unsigned int fromIndex = 0;

    for (unsigned int i = 0; i < size; i++) {
        const TokenType currentTokenType = tokens.at(i).type;
        if (currentTokenType == separator) {
            const unsigned int toIndex = i;
            std::vector<Token>::const_iterator first = tokens.begin() + fromIndex;
            std::vector<Token>::const_iterator last = tokens.begin() + toIndex;
            lines.emplace_back(first, last);
            fromIndex = toIndex + 1;
        }
    }

    return lines;
}


unsigned int SyntaxAnalyzer::getShiftLevel(const std::vector<Token>& line) const {
    unsigned int shifts = 0;
    for (unsigned int i = 0; i < line.size(); i++) {
        const TokenType currentTokenType = line.at(i).type;
        if (currentTokenType == TokenType_Tab) {
            shifts++;
        } else {
            return shifts;
        }
    }

    return shifts;
}


std::vector<Token> SyntaxAnalyzer::stripFromTabs(const std::vector<Token>& line) const {
    std::vector<Token> strippedLine;
    for (const Token& token : line) {
        const TokenType currentTokenType = token.type;
        if (currentTokenType != TokenType_Tab) {
            strippedLine.push_back(token);
        }
    }

    return strippedLine;
}


bool SyntaxAnalyzer::areParenthesesCorrect(const std::vector<Token>& line) const {
    const unsigned int parenthesesCode = 0; // ()

    int stack[line.size()]; // there can be no more than that many elements
    int stackTop = -1;

    for (const Token& token : line) {
        const TokenType tokenType = token.type;
        switch (tokenType) {
            case TokenType_LeftParenthesis:
                stack[++stackTop] = parenthesesCode;
                break;
            case TokenType_RightParenthesis:
                if (stackTop < 0 || stack[stackTop] != parenthesesCode) {
                    std::cout << "Syntax Error: Unmatched parentheses ')'. "
                              << std::endl;
                    return false;
                }
                stackTop--;
                break;
            default:
                break;
        }
    }

    if (stackTop != -1) {
        switch (stack[0]) {
            case parenthesesCode:
                std::cout << "Syntax Error: Unmatched parentheses '('. "
                          << std::endl;
                break;
            default:
                break;
        }
    }

    return (stackTop == -1);
}


std::map<std::string, Type> SyntaxAnalyzer::getVariablesTable() const {
    return m_VariablesTable;
}


std::vector<Node> SyntaxAnalyzer::getNodes() const {
    return m_Nodes;
}

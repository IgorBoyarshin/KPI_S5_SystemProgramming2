#include "LexicalAnalyzer.h"


LexicalAnalyzer::LexicalAnalyzer() : m_TokensTable(std::vector<Token>()) {}


std::vector<Token> LexicalAnalyzer::analyze(const std::string& input) {
    const unsigned int size = input.size();
    if (size == 0) {
        return m_TokensTable;
    }

    unsigned int currentIndex = 0;
    std::string accumulator("");
    bool accumulatingNumber;

    while (currentIndex < size) {
        const char c = input[currentIndex];
        std::string cc;
        TokenType tokenType;
        bool separatorFound = false;

        switch(c) {
            case '(':
                tokenType = TokenType_Parentheses_Open;
                separatorFound = true;
                break;
            case ')':
                tokenType = TokenType_Parentheses_Close;
                separatorFound = true;
                break;

            case '{':
                tokenType = TokenType_Braces_Open;
                separatorFound = true;
                break;
            case '}':
                tokenType = TokenType_Braces_Close;
                separatorFound = true;
                break;

            case '[':
                tokenType = TokenType_Brackets_Open;
                separatorFound = true;
                break;
            case ']':
                tokenType = TokenType_Brackets_Close;
                separatorFound = true;
                break;

            // TODO: implement unary !
            case '!':
                if (currentIndex + 1 < size - 1 && input[currentIndex + 1] == '=') {
                    currentIndex++;
                    tokenType = TokenType_Operator;
                    cc = "!=";
                } else {
                    tokenType = TokenType_Unknown;
                }
                separatorFound = true;
                break;

            case '=':
                if (currentIndex + 1 < size - 1 && input[currentIndex + 1] == '=') {
                    currentIndex++;
                    tokenType = TokenType_Operator;
                    cc = "==";
                } else {
                    tokenType = TokenType_Operator_Assign;
                }
                separatorFound = true;
                break;

            case '<':
                if (currentIndex + 1 < size - 1 && input[currentIndex + 1] == '=') {
                    currentIndex++;
                    cc = "<=";
                }
                tokenType = TokenType_Operator;
                separatorFound = true;
                break;
            case '>':
                if (currentIndex + 1 < size - 1 && input[currentIndex + 1] == '=') {
                    currentIndex++;
                    cc = ">=";
                }
                tokenType = TokenType_Operator;
                separatorFound = true;
                break;

            case '+':
            case '-':
            case '*':
            case '/':
                tokenType = TokenType_Operator;
                separatorFound = true;
                break;

            case ';':
                tokenType = TokenType_Semicolon;
                separatorFound = true;
                break;

            case ',':
                tokenType = TokenType_Comma;
                separatorFound = true;
                break;

            case ' ':
                separatorFound = true;
                break;

            default:
                separatorFound = false;
                if (isAlpha(c)) {
                    if (accumulator.size() == 0) {
                        accumulatingNumber = false;
                    } else if (accumulatingNumber) {
                        // something like "35var" => invalid
                        std::cout << ":> Bad format(at char " << currentIndex
                                  << "): " << accumulator << std::endl;
                        return m_TokensTable;
                    }

                    accumulator += c;
                } else if (isNumber(c) || c == '.') {
                    if (accumulator.size() == 0) {
                        accumulatingNumber = true;
                    }

                    accumulator += c;
                } else {
                    std::cout << ":> Bad format: Unknown char at "
                              << currentIndex << ": " << c << std::endl;
                    return m_TokensTable;
                }
        }

        if (separatorFound) {
            const bool accumulating = accumulator.size() > 0;
            if (accumulating) {
                // flush the accumulator
                if (accumulatingNumber) {
                    if (accumulator.find('.') != std::string::npos) {
                        // TODO: prone to be incorrect
                        m_TokensTable.emplace_back(TokenType_LiteralFloat, accumulator);
                    } else {
                        m_TokensTable.emplace_back(TokenType_LiteralInt, accumulator);
                    }
                } else {
                    if (accumulator.compare("true") == 0 ||
                            accumulator.compare("false") == 0) {
                        m_TokensTable.emplace_back(TokenType_LiteralBool, accumulator);
                    } else {
                        const TokenType keyword = getIfKeyword(accumulator);
                        m_TokensTable.emplace_back(
                                keyword != TokenType_Unknown ? keyword : TokenType_Identifier,
                                accumulator);
                    }
                }

                accumulator.clear();
            }

            // Regardless of whether we were accumulating or not,
            // It is time to insert the signle token
            // (unless it is a space, in this case just skip it)
            if (c != ' ') {
                if (cc.size() > 0) {
                    m_TokensTable.push_back(Token(tokenType, cc));
                } else {
                    m_TokensTable.push_back(Token(tokenType, std::string(1, c)));
                }
                // m_TokensTable.emplace_back(tokenType, "" + c);
            }

        }
        // If separatorFound == false => we either don't care or it's impossible

        currentIndex++;
    }

    return m_TokensTable;
}

std::ostream& operator<<(std::ostream& os, const LexicalAnalyzer& lexicalAnalyzer) {
    os << "Tokens table:" << std::endl;

    for (const Token& token : lexicalAnalyzer.m_TokensTable) {
        os << token << std::endl;
    }

    return os;
}


bool isAlpha(char c) {
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

bool isNumber(char c) {
    return (c >= '0' && c <= '9');
}

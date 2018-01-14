#include "LexicalAnalyzer.h"


LexicalAnalyzer::LexicalAnalyzer() : m_TokensTable(std::vector<Token>()) {}


std::vector<Token> LexicalAnalyzer::analyze(const std::string& input) {
    const unsigned int size = input.size();
    if (size == 0) {
        return std::vector<Token>();
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
                tokenType = TokenType_LeftParenthesis;
                separatorFound = true;
                break;
            case ')':
                tokenType = TokenType_RightParenthesis;
                separatorFound = true;
                break;

            case '=':
                if (currentIndex + 1 < size && input[currentIndex + 1] == '=') {
                    currentIndex++;
                    tokenType = TokenType_Equals;
                    cc = "==";
                } else {
                    tokenType = TokenType_Assign;
                }
                separatorFound = true;
                break;

            case '<':
                if (currentIndex + 1 < size && input[currentIndex + 1] == '=') {
                    currentIndex++;
                    cc = "<=";
                    tokenType = TokenType_LessOrEquals;
                } else {
                    tokenType = TokenType_Less;
                }
                separatorFound = true;
                break;
            case '>':
                if (currentIndex + 1 < size - 1 && input[currentIndex + 1] == '=') {
                    currentIndex++;
                    cc = ">=";
                    tokenType = TokenType_GreaterOrEquals;
                } else {
                    tokenType = TokenType_Greater;
                }
                separatorFound = true;
                break;

            case '+':
                tokenType = TokenType_Plus;
                separatorFound = true;
                break;
            case '-':
                tokenType = TokenType_Minus;
                separatorFound = true;
                break;
            case '*':
                tokenType = TokenType_Multiply;
                separatorFound = true;
                break;
            case '/':
                tokenType = TokenType_Divide;
                separatorFound = true;
                break;

            case ':':
                tokenType = TokenType_Colon;
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

            case '\t':
                tokenType = TokenType_Tab;
                cc = "\\t";
                separatorFound = true;
                break;
            case '\n':
                tokenType = TokenType_NewLine;
                cc = "\\n";
                separatorFound = true;
                break;

            default:
                separatorFound = false;
                if (isAlpha(c)) {
                    if (accumulator.size() == 0) {
                        accumulatingNumber = false;
                    } else if (accumulatingNumber) {
                        // something like "35var" => invalid
                        std::cout << "LexicalAnalyzer Error: Invalid number at char " << currentIndex
                                  << "." << std::endl;
                        return std::vector<Token>();
                    }

                    accumulator += c;
                } else if (isNumber(c) || c == '.') {
                    if (accumulator.size() == 0) {
                        accumulatingNumber = true;
                    }

                    accumulator += c;
                } else {
                    std::cout << "LexicalAnalyzer Error: Unknown char at "
                              << currentIndex << ": " << c << std::endl;
                    return std::vector<Token>();
                }
        }

        if (separatorFound) {
            const bool accumulating = accumulator.size() > 0;
            if (accumulating) {
                // flush the accumulator
                if (accumulatingNumber) {
                    m_TokensTable.emplace_back(TokenType_Number, accumulator);
                } else {
                    if (accumulator.compare("True") == 0 ||
                            accumulator.compare("False") == 0) {
                        m_TokensTable.emplace_back(TokenType_Bool, accumulator);
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
            // It is time to insert the single token
            // (unless it is a space, in whice case just skip it)
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

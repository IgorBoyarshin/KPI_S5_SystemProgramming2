#include "LexicalAnalyzer.h"
#include <string>

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
        TokenName tokenName;
        bool separatorFound = false;

        switch(c) {
            case '(':
                tokenName = TokenName_Parentheses_Open;
                separatorFound = true;
                break;
            case ')':
                tokenName = TokenName_Parentheses_Close;
                separatorFound = true;
                break;

            case '{':
                tokenName = TokenName_Braces_Open;
                separatorFound = true;
                break;
            case '}':
                tokenName = TokenName_Braces_Close;
                separatorFound = true;
                break;

            case '[':
                tokenName = TokenName_Brackets_Open;
                separatorFound = true;
                break;
            case ']':
                tokenName = TokenName_Brackets_Close;
                separatorFound = true;
                break;

            // TODO: implement unary !
            case '!':
                if (currentIndex + 1 < size - 1 && input[currentIndex + 1] == '=') {
                    currentIndex++;
                    tokenName = TokenName_Operator;
                    cc = "!=";
                } else {
                    tokenName = TokenName_Unknown;
                }
                separatorFound = true;
                break;

            case '=':
                if (currentIndex + 1 < size - 1 && input[currentIndex + 1] == '=') {
                    currentIndex++;
                    tokenName = TokenName_Operator;
                    cc = "==";
                } else {
                    tokenName = TokenName_Operator_Assign;
                }
                separatorFound = true;
                break;

            case '+':
            case '-':
            case '*':
            case '/':
                tokenName = TokenName_Operator;
                separatorFound = true;
                break;

            case ';':
                tokenName = TokenName_Semicolon;
                separatorFound = true;
                break;

            case ',':
                tokenName = TokenName_Comma;
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
                } else if (isNumber(c)) {
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
                    m_TokensTable.emplace_back(TokenName_Literal, accumulator);
                } else {
                    const TokenName keyword = getIfKeyword(accumulator);
                    m_TokensTable.emplace_back(
                            keyword != TokenName_Unknown ? keyword : TokenName_Identifier,
                            accumulator);
                }

                accumulator.clear();
            }

            // Regardless of whether we were accumulating or not,
            // It is time to insert the signle token
            // (unless it is a space, in this case just skip it)
            if (c != ' ') {
                if (cc.size() > 0) {
                    m_TokensTable.push_back(Token(tokenName, cc));
                } else {
                    m_TokensTable.push_back(Token(tokenName, std::string(1, c)));
                }
                // m_TokensTable.emplace_back(tokenName, "" + c);
            }

        }
        // If separatorFound == false => we either don't care or it's impossible

        currentIndex++;
    }

    return m_TokensTable;
}


bool LexicalAnalyzer::insert(unsigned int index, const Token& token) {
    if (index >= m_TokensTable.size()) {
        return false;
    }

    m_TokensTable.insert(m_TokensTable.begin() + index, token);

    return true;
}


bool LexicalAnalyzer::update(unsigned int index, const Token& token) {
    if (index >= m_TokensTable.size()) {
        return false;
    }

    m_TokensTable[index] = token;

    return true;
}


Token* LexicalAnalyzer::select(unsigned int index) {
    if (index >= m_TokensTable.size()) {
        return nullptr;
    }

    return &(m_TokensTable[index]);
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

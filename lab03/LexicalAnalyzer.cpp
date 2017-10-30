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
        TokenName tokenName;
        bool separatorFound = false;

        switch(c) {
            case '(':
            case ')':
                tokenName = TokenName_Parentheses;
                separatorFound = true;
                break;

            case '{':
            case '}':
                tokenName = TokenName_Braces;
                separatorFound = true;
                break;

            case '[':
            case ']':
                tokenName = TokenName_Brackets;
                separatorFound = true;
                break;

            case '=':
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
                    const bool keyword = isKeyword(accumulator);
                    m_TokensTable.emplace_back(
                            keyword ? TokenName_Keyword : TokenName_Identifier,
                            accumulator);
                }

                accumulator.clear();
            }

            // Regardless of whether we were accumulating or not,
            // It is time to insert the signle token
            // (unless it is a space, in this case just skip it)
            if (c != ' ') {
                m_TokensTable.push_back(Token(tokenName, std::string(1, c)));
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

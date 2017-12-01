#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include <vector>
#include <string>
#include <string>
#include "TokenType.h"
#include "Token.h"


class LexicalAnalyzer {
    public:
        LexicalAnalyzer();

    public:
        std::vector<Token> analyze(const std::string& input);

        friend std::ostream& operator<<(
            std::ostream& os,
            const LexicalAnalyzer& lexicalAnalyzer);


    private:
        std::vector<Token> m_TokensTable;
};

std::ostream& operator<<(std::ostream& os, const LexicalAnalyzer& lexicalAnalyzer);

bool isAlpha(char c);
bool isNumber(char c);

#endif

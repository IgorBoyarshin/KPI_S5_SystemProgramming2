#ifndef SYNTAX_ANALYZER_H
#define SYNTAX_ANALYZER_H

#include <vector>
#include <string>
#include "Token.h"
#include "Context.h"
#include "Node.h"

class SyntaxAnalyzer {
    public:
        SyntaxAnalyzer();
        SyntaxAnalyzer(const std::vector<Token> tokensTable);

        bool parse();
    private:
        const std::vector<Token> m_TokensTable;

        std::vector<Node*> nodes;
        unsigned int currentTokensTableIndex;
        Context currentContext;


    private:
        // returns true if the reduce was possible
        bool reduceIfCan();

        const Token* getNext();

        const Token* peekNext() const;

        bool isAllowed(const Token* token1, const Token* token2) const;

        bool matchExists(const std::string& str1, const std::string& str2) const;
};

#endif

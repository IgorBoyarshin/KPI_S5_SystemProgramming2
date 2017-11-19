#ifndef SYNTAX_ANALYZER_H
#define SYNTAX_ANALYZER_H

#include <vector>
#include <string>
#include "Token.h"
#include "Context.h"
#include "ContextType.h"
#include "Node.h"

class SyntaxAnalyzer {
    public:
        SyntaxAnalyzer();
        SyntaxAnalyzer(const std::vector<Token> tokensTable);

        bool parse();
    private:
        const std::vector<Token> m_TokensTable;

        std::vector<Node*> m_Nodes;
        unsigned int m_CurrentTokensTableIndex;
        Context m_CurrentContext;


    private:
        // returns true if the reduce was possible
        bool reduceIfCan();

        const Token* getNext();

        const Token* peekNext() const;

        bool isAllowed(const NodeType nodeType1, const NodeType nodeType2, const Context& context) const;

        bool matchExists(const std::string& str1, const std::string& str2) const;

        Node convertIntoNodeType(const Token* token) const;
};

#endif

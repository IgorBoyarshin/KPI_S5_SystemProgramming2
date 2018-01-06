#ifndef SYNTAX_ANALYZER_H
#define SYNTAX_ANALYZER_H

#include <vector>
#include <string>
#include "Token.h"
// #include "Context.h"
// #include "ContextType.h"
#include "Node.h"
#include "RuleType.h"
#include <tuple>
#include <map>


typedef std::tuple<std::vector<NodeType>, NodeType, RuleType> ReductionPattern;

class SyntaxAnalyzer {
    public:
        SyntaxAnalyzer();
        SyntaxAnalyzer(const std::vector<Token> tokensTable);

        bool parse();
    private:
        std::vector<Node> m_ConvertedTokensTable;

        std::vector<const Node*> m_Nodes;
        unsigned int m_NextTokensTableIndex;
        // Context m_CurrentContext;

        std::vector<ReductionPattern> m_ReductionPatterns;
        std::map<NodeType, std::vector<NodeType>> m_AllowedSequences;


    private:
        // returns true if the reduce was possible
        bool reduceIfCan();

        std::vector<ReductionPattern> getPotentialReductionPatterns() const;

        const Node* getNext();

        const Node* peekNext() const;

        bool isAllowed(const NodeType nodeType1, const NodeType nodeType2) const; //, const Context& context) const;

        bool matchExists(const std::string& str1, const std::string& str2) const;

        Node convertIntoNodeType(const Token token) const;
};

#endif

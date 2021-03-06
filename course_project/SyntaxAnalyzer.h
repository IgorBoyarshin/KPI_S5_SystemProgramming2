#ifndef SYNTAX_ANALYZER_H
#define SYNTAX_ANALYZER_H

#include <vector>
#include <fstream>
#include <sstream>
#include <iterator>
#include <string>
#include <tuple>
#include <map>
#include "Token.h"
#include "Node.h"


typedef std::tuple<std::vector<NodeType>, NodeType> ReductionPattern;

class SyntaxAnalyzer {
    public:
        SyntaxAnalyzer();
        SyntaxAnalyzer(const std::vector<Token> tokensTable);

        bool parse();
        const Node* getRootNode() const;
    private:
        std::vector<Node> m_ConvertedTokensTable;

        std::vector<const Node*> m_Nodes;
        unsigned int m_NextTokensTableIndex;

        // <id name> <-> [return type|variable type, argument types..]
        std::map<std::string, std::vector<NodeType>> m_IdsTable;

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


std::vector<ReductionPattern> readReductionPatterns(const std::string& fileName);
std::map<NodeType, std::vector<NodeType>> readAllowedSequences(const std::string& fileName);

#endif

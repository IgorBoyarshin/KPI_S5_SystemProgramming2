#include "SyntaxAnalyzer.h"
#include "Token.h"
#include <vector>

SyntaxAnalyzer::SyntaxAnalyzer(
        const std::vector<Token> tokensTable) : m_TokensTable(tokensTable) {}


bool SyntaxAnalyzer::parse() {
    Token* tokenPrevious = nullptr;
    // Token* tokenCurrent = nullptr;
    while (peekNext()) {
        const Token* tokenCurrent = getNext();
        if (!isAllowed(tokenPrevious, tokenCurrent)) {
            std::cout << "[Parse error]: Unexpected token \""
                      << tokenCurrent << "\" after \""
                      << tokenPrevious << "\"." << std::endl;
        }

        while(reduceIfCan());
    }

    if (nodes.size() > 1) {
        std::cout << "[Parse error]: Unexpected input end." << std::endl;
        return false;
    }

    if (nodes[0]->m_NodeType != NodeType_Statement) {
        std::cout << "[Parse error]: Unexpected top-level entry." << std::endl;
        return false;
    }

    return true;
}


bool SyntaxAnalyzer::isAllowed(const Token* token1, const Token* token2) const {

}

#include "SyntaxAnalyzer.h"
#include "Token.h"
#include <vector>

SyntaxAnalyzer::SyntaxAnalyzer(
        const std::vector<Token> tokensTable) : m_TokensTable(tokensTable) {}


Node SyntaxAnalyzer::convertIntoNodeType(const Token* token) const {
    const TokenName tokenName = token->getName();
    const std::string tokenValue = token->getValue();
    switch(tokenName) {
        case TokenName_Identifier:
            return Node(NodeType_Id, tokenValue, std::vector<Node*>());
        case TokenName_Literal:
            return Node(NodeType_Literal, tokenValue, std::vector<Node*>());
        case TokenName_Keyword_If:
            return Node(NodeType_KeywordIf, tokenValue, std::vector<Node*>());
        case TokenName_Keyword_Else:
            return Node(NodeType_KeywordElse, tokenValue, std::vector<Node*>());
        case TokenName_Operator:
            if (tokenValue.compare("+") == 0) {
                return Node(NodeType_OperatorPlus, tokenValue, std::vector<Node*>());
            } else if (tokenValue.compare("-") == 0) {
                return Node(NodeType_OperatorMinus, tokenValue, std::vector<Node*>());
            } else if (tokenValue.compare("*") == 0) {
                return Node(NodeType_OperatorMultiply, tokenValue, std::vector<Node*>());
            } else if (tokenValue.compare("/") == 0) {
                return Node(NodeType_OperatorDivide, tokenValue, std::vector<Node*>());
            } else {
                std::cout << ":> Error: Unknown operator in convertIntoNodeType(): "
                    << tokenValue << std::endl;
            }
        case TokenName_Operator_Equals:
            return Node(NodeType_OperatorAssign, tokenValue, std::vector<Node*>());
        case TokenName_Brackets_Open:
            return Node(NodeType_BracketsOpen, tokenValue, std::vector<Node*>());
        case TokenName_Brackets_Close:
            return Node(NodeType_BracketsClose, tokenValue, std::vector<Node*>());
        case TokenName_Parentheses_Open:
            return Node(NodeType_ParenthesesOpen, tokenValue, std::vector<Node*>());
        case TokenName_Parentheses_Close:
            return Node(NodeType_ParenthesesClose, tokenValue, std::vector<Node*>());
        case TokenName_Braces_Open:
            return Node(NodeType_BracesOpen, tokenValue, std::vector<Node*>());
        case TokenName_Braces_Close:
            return Node(NodeType_BracesClose, tokenValue, std::vector<Node*>());
        case TokenName_Semicolon:
            return Node(NodeType_Semicolon, tokenValue, std::vector<Node*>());
        case TokenName_Comma:
            return Node(NodeType_Comma, tokenValue, std::vector<Node*>());
        case TokenName_Unknown:
        default:
            std::cout << ":> Error: Encountered Unknown Token in convertIntoNodeType():"
                << tokenValue;
    }

    return Node(NodeType_Semicolon, tokenValue, std::vector<Node*>());
}


bool SyntaxAnalyzer::parse() {
    // Node* nodePrevious = nullptr;
    while (peekNext()) {
        const Node* nodePrevious = 
            m_Nodes.size() == 0 ? nullptr : m_Nodes[m_Nodes.size() - 1];
        const Node nodeCurrent = convertIntoNodeType(getNext());
        if (!isAllowed(
                nodePrevious->m_NodeType, 
                nodeCurrent.m_NodeType, 
                m_CurrentContext)) {

            std::cout << "[Parse error]: Unexpected token \""
                      << *nodePrevious << "\" after \""
                      << nodeCurrent << "\"." << std::endl;
        }

        while(reduceIfCan());
    }

    if (m_Nodes.size() > 1) {
        std::cout << "[Parse error]: Unexpected input end." << std::endl;
        return false;
    }

    if (m_Nodes[0]->m_NodeType != NodeType_Statement) {
        std::cout << "[Parse error]: Unexpected top-level entry." << std::endl;
        return false;
    }

    return true;
}


bool SyntaxAnalyzer::isAllowed(
        const NodeType nodeType1, 
        const NodeType nodeType2,
        const Context& context) const {

    switch (nodeType1) {
        case NodeType_Id:
            switch (nodeType2) {
                case NodeType_ParenthesesOpen:
                case NodeType_ParenthesesClose:
                case NodeType_OperatorPlus:
                case NodeType_OperatorMinus:
                case NodeType_OperatorMultiply:
                case NodeType_OperatorDivide:
                case NodeType_OperatorAssign:
                case NodeType_OperatorEquals:
                case NodeType_OperatorNotEquals:
                    return true;
                case NodeType_Comma:
                    return context.isInFunction();
                case NodeType_Semicolon:
                    return !context.isInFunction();
                default:
                    return false;
            }

        case NodeType_Literal:
            switch (nodeType2) {
                case NodeType_ParenthesesClose:
                case NodeType_OperatorPlus:
                case NodeType_OperatorMinus:
                case NodeType_OperatorMultiply:
                case NodeType_OperatorDivide:
                case NodeType_OperatorEquals:
                case NodeType_OperatorNotEquals:
                    return true;
                case NodeType_Comma:
                    return context.isInFunction();
                case NodeType_Semicolon:
                    return !context.isInFunction();
                default:
                    return false;
            }

        case NodeType_KeywordIf:
            switch (nodeType2) {
                case NodeType_ParenthesesOpen:
                    return true;
                default:
                    return false;
            }

        case NodeType_KeywordElse:
            switch (nodeType2) {
                case NodeType_Statement:
                    return true;
                default:
                    return false;
            }

        case NodeType_BracketsOpen:
        case NodeType_BracketsClose:
            return false;

        case NodeType_ParenthesesOpen:
            switch (nodeType2) {
                case NodeType_Id:
                case NodeType_Literal:
                case NodeType_ParenthesesOpen:
                    return true;
                case NodeType_ParenthesesClose:
                    return context.isInFunction();
                case NodeType_Expression:
                case NodeType_Function:
                    return true;
                default:
                    return false;
            }

        case NodeType_ParenthesesClose:
            switch (nodeType2) {
                case NodeType_Id:
                case NodeType_KeywordIf:
                    return context.isInIfStatement();
                case NodeType_ParenthesesClose:
                    return true;
                case NodeType_BracesOpen:
                    return context.isInIfStatement();
                case NodeType_OperatorPlus:
                case NodeType_OperatorMinus:
                case NodeType_OperatorMultiply:
                case NodeType_OperatorDivide:
                case NodeType_OperatorEquals:
                case NodeType_OperatorNotEquals:
                    return context.isInExpression();
                case NodeType_Comma:
                    return context.isInFunction();
                case NodeType_Semicolon:
                    return !context.isInIfStatement();
                case NodeType_Statement:
                case NodeType_StatementBody:
                case NodeType_Function:
                    return context.isInIfStatement();
                default:
                    return false;
            }

        case NodeType_BracesOpen:
            switch (nodeType2) {
                case NodeType_Id:
                case NodeType_KeywordIf:
                case NodeType_BracesOpen:
                case NodeType_BracesClose:
                case NodeType_Semicolon: // ??
                case NodeType_Statement:
                case NodeType_StatementBody:
                case NodeType_Function:
                    return true;
                default:
                    return false;
            }

        case NodeType_BracesClose:
            switch (nodeType2) {
                case NodeType_Id:
                case NodeType_KeywordIf:
                case NodeType_KeywordElse:
                case NodeType_BracesOpen:
                case NodeType_BracesClose:
                case NodeType_Semicolon:
                case NodeType_Statement:
                case NodeType_StatementBody:
                case NodeType_Function:
                    return true;
                default:
                    return false;
            }

        case NodeType_OperatorPlus:
            switch (nodeType2) {
                case NodeType_Id:
                case NodeType_Literal:
                case NodeType_ParenthesesOpen:
                case NodeType_Expression:
                case NodeType_Function:
                    return true;
                default:
                    return false;
            }

        case NodeType_OperatorMinus:
            switch (nodeType2) {
                case NodeType_Id:
                case NodeType_Literal:
                case NodeType_ParenthesesOpen:
                case NodeType_Expression:
                case NodeType_Function:
                    return true;
                default:
                    return false;
            }

        case NodeType_OperatorMultiply:
            switch (nodeType2) {
                case NodeType_Id:
                case NodeType_Literal:
                case NodeType_ParenthesesOpen:
                case NodeType_Expression:
                case NodeType_Function:
                    return true;
                default:
                    return false;
            }

        case NodeType_OperatorDivide:
            switch (nodeType2) {
                case NodeType_Id:
                case NodeType_Literal:
                case NodeType_ParenthesesOpen:
                case NodeType_Expression:
                case NodeType_Function:
                    return true;
                default:
                    return false;
            }

        case NodeType_OperatorEquals:
            switch (nodeType2) {
                case NodeType_Id:
                case NodeType_Literal:
                case NodeType_ParenthesesOpen:
                case NodeType_Expression:
                case NodeType_Function:
                    return true;
                default:
                    return false;
            }

        case NodeType_OperatorNotEquals:
            switch (nodeType2) {
                case NodeType_Id:
                case NodeType_Literal:
                case NodeType_ParenthesesOpen:
                case NodeType_Expression:
                case NodeType_Function:
                    return true;
                default:
                    return false;
            }

        case NodeType_OperatorAssign:
            switch (nodeType2) {
                case NodeType_Id:
                case NodeType_Literal:
                case NodeType_ParenthesesOpen:
                case NodeType_Expression:
                case NodeType_Function:
                    return true;
                default:
                    return false;
            }

        case NodeType_Comma:
            switch (nodeType2) {
                case NodeType_Id:
                case NodeType_Function:
                case NodeType_Literal:
                case NodeType_ParenthesesOpen:
                case NodeType_Expression:
                    return context.isInFunction();
                default:
                    return false;
            }

        case NodeType_Semicolon:
            switch (nodeType2) {
                case NodeType_KeywordIf:
                case NodeType_KeywordElse:
                case NodeType_Id:
                case NodeType_Function:
                case NodeType_Statement:
                case NodeType_StatementBody:
                case NodeType_ParenthesesOpen:
                case NodeType_ParenthesesClose:
                    return true;
                default:
                    return false;
            }

        case NodeType_Statement:
            switch (nodeType2) {
                case NodeType_Id:
                case NodeType_KeywordIf:
                case NodeType_KeywordElse:
                case NodeType_BracesOpen:
                case NodeType_BracesClose:
                case NodeType_Semicolon:
                case NodeType_Statement:
                case NodeType_StatementBody:
                case NodeType_Function:
                    return true;
                default:
                    return false;
            }

        case NodeType_StatementBody:
            switch (nodeType2) {
                case NodeType_Semicolon:
                    return true;
                default:
                    return false;
            }

        case NodeType_Expression:
            switch (nodeType2) {
                case NodeType_ParenthesesClose:
                case NodeType_OperatorPlus:
                case NodeType_OperatorMinus:
                case NodeType_OperatorMultiply:
                case NodeType_OperatorDivide:
                case NodeType_OperatorEquals:
                case NodeType_OperatorNotEquals:
                    return true;
                case NodeType_Comma:
                    return context.isInFunction();
                case NodeType_Semicolon:
                    return !context.isInFunction() && context.isInStatementBody();
                default:
                    return false;
            }

        case NodeType_Function:
            switch (nodeType2) {
                case NodeType_ParenthesesClose:
                case NodeType_OperatorPlus:
                case NodeType_OperatorMinus:
                case NodeType_OperatorMultiply:
                case NodeType_OperatorDivide:
                case NodeType_OperatorEquals:
                case NodeType_OperatorNotEquals:
                    return true;
                case NodeType_Comma:
                    return context.isInFunction();
                case NodeType_Semicolon:
                    return !context.isInFunction() && context.isInStatementBody();
                default:
                    return false;
            }

        default:
            std::cout << ":> ERROR: Unexpected NodeType: "
                << nodeType1;
    }

    return false;




// BLUEPRINT START ------------
            // switch (nodeType2) {
            //     case NodeType_Id:
            //     case NodeType_Literal:
            //     case NodeType_KeywordIf:
            //     case NodeType_KeywordElse:
            //     case NodeType_BracketsOpen:
            //     case NodeType_BracketsClose:
            //     case NodeType_ParenthesesOpen:
            //     case NodeType_ParenthesesClose:
            //     case NodeType_BracesOpen:
            //     case NodeType_BracesClose:
            //     case NodeType_OperatorPlus:
            //     case NodeType_OperatorMinus:
            //     case NodeType_OperatorMultiply:
            //     case NodeType_OperatorDivide:
            //     case NodeType_OperatorEquals:
            //     case NodeType_OperatorNotEquals:
            //     case NodeType_OperatorAssign:
            //     case NodeType_Comma:
            //     case NodeType_Semicolon:
            //     case NodeType_Statement:
            //     case NodeType_StatementBody:
            //     case NodeType_Expression:
            //     case NodeType_Function:
            //         return true;
            //     default:
            //         return false;
            // }
//
// BLUEPRINT END --------------

}

#include "SyntaxAnalyzer.h"
#include "Token.h"
#include <vector>

SyntaxAnalyzer::SyntaxAnalyzer(
        const std::vector<Token> tokensTable) :
    m_ConvertedTokensTable(std::vector<Node>()),
    m_NextTokensTableIndex(0),
    // m_CurrentContext(Context()),
    m_ReductionPatterns(std::vector<ReductionPattern>()),
    m_AllowedSequences(std::map<NodeType, std::vector<NodeType>>())
{
    // Convert all tokens into their Node equivalents
    for (Token token : tokensTable) {
        m_ConvertedTokensTable.push_back(convertIntoNodeType(token));
    }


    // Now generate m_AllowedSequences

    m_AllowedSequences.insert(
        std::pair<NodeType, std::vector<NodeType>> (
            NodeType_Statement,
            {NodeType_Statement, NodeType_KeywordElse, NodeType_BracesClose}
        )
    );
    m_AllowedSequences.insert(
        std::pair<NodeType, std::vector<NodeType>> (
            NodeType_StatementList,
            {NodeType_Statement, NodeType_BracesClose}
        )
    );
    m_AllowedSequences.insert(
        std::pair<NodeType, std::vector<NodeType>> (
            NodeType_Expression,
            {NodeType_Semicolon, NodeType_ParenthesesClose, NodeType_OperatorPlus,
                NodeType_OperatorMinus, NodeType_OperatorMultiply, NodeType_OperatorDivide,
                NodeType_OperatorEquals, NodeType_OperatorNotEquals, NodeType_OperatorLess,
                NodeType_OperatorLessOrEquals, NodeType_OperatorGreater,
                NodeType_OperatorGreaterOrEquals, NodeType_Comma}
        )
    );
    m_AllowedSequences.insert(
        std::pair<NodeType, std::vector<NodeType>> (
            NodeType_ExpressionList,
            {NodeType_ParenthesesClose, NodeType_Comma}
        )
    );
    m_AllowedSequences.insert(
        std::pair<NodeType, std::vector<NodeType>> (
            NodeType_Function,
            {NodeType_Semicolon}
        )
    );
    m_AllowedSequences.insert(
        std::pair<NodeType, std::vector<NodeType>> (
            NodeType_Id,
            {NodeType_OperatorAssign, NodeType_ParenthesesOpen}
        )
    );
    m_AllowedSequences.insert(
        std::pair<NodeType, std::vector<NodeType>> (
            NodeType_KeywordIf,
            {NodeType_ParenthesesOpen}
        )
    );
    m_AllowedSequences.insert(
        std::pair<NodeType, std::vector<NodeType>> (
            NodeType_KeywordElse,
            {NodeType_Statement}
        )
    );
    m_AllowedSequences.insert(
        std::pair<NodeType, std::vector<NodeType>> (
            NodeType_OperatorAssign,
            {NodeType_Expression}
        )
    );
    m_AllowedSequences.insert(
        std::pair<NodeType, std::vector<NodeType>> (
            NodeType_OperatorPlus,
            {NodeType_Expression}
        )
    );
    m_AllowedSequences.insert(
        std::pair<NodeType, std::vector<NodeType>> (
            NodeType_OperatorMinus,
            {NodeType_Expression}
        )
    );
    m_AllowedSequences.insert(
        std::pair<NodeType, std::vector<NodeType>> (
            NodeType_OperatorMultiply,
            {NodeType_Expression}
        )
    );
    m_AllowedSequences.insert(
        std::pair<NodeType, std::vector<NodeType>> (
            NodeType_OperatorDivide,
            {NodeType_Expression}
        )
    );
    m_AllowedSequences.insert(
        std::pair<NodeType, std::vector<NodeType>> (
            NodeType_OperatorEquals,
            {NodeType_Expression}
        )
    );
    m_AllowedSequences.insert(
        std::pair<NodeType, std::vector<NodeType>> (
            NodeType_OperatorNotEquals,
            {NodeType_Expression}
        )
    );
    m_AllowedSequences.insert(
        std::pair<NodeType, std::vector<NodeType>> (
            NodeType_OperatorLess,
            {NodeType_Expression}
        )
    );
    m_AllowedSequences.insert(
        std::pair<NodeType, std::vector<NodeType>> (
            NodeType_OperatorLessOrEquals,
            {NodeType_Expression}
        )
    );
    m_AllowedSequences.insert(
        std::pair<NodeType, std::vector<NodeType>> (
            NodeType_OperatorGreater,
            {NodeType_Expression}
        )
    );
    m_AllowedSequences.insert(
        std::pair<NodeType, std::vector<NodeType>> (
            NodeType_OperatorGreaterOrEquals,
            {NodeType_Expression}
        )
    );
    m_AllowedSequences.insert(
        std::pair<NodeType, std::vector<NodeType>> (
            NodeType_BracesOpen,
            {NodeType_BracesClose, NodeType_StatementList, NodeType_Statement}
        )
    );
    m_AllowedSequences.insert(
        std::pair<NodeType, std::vector<NodeType>> (
            NodeType_ParenthesesOpen,
            {NodeType_ParenthesesClose, NodeType_Expression, NodeType_ExpressionList}
        )
    );
    m_AllowedSequences.insert(
        std::pair<NodeType, std::vector<NodeType>> (
            NodeType_ParenthesesClose,
            {NodeType_Statement}
        )
    );
    m_AllowedSequences.insert(
        std::pair<NodeType, std::vector<NodeType>> (
            NodeType_Comma,
            {NodeType_Expression}
        )
    );


    // Now generete rules

    // Function
    m_ReductionPatterns.push_back(
        std::make_tuple<std::vector<NodeType>, NodeType, RuleType>(
            {NodeType_Id, NodeType_ParenthesesOpen, NodeType_ParenthesesClose},
            NodeType_Function,
            RuleType_Function_IdParentheses
        )
    );
    m_ReductionPatterns.push_back(
        std::make_tuple<std::vector<NodeType>, NodeType, RuleType>(
            {NodeType_Id, NodeType_ParenthesesOpen, NodeType_ExpressionList, NodeType_ParenthesesClose},
            NodeType_Function,
            RuleType_Function_IdParenthesesBlock
        )
    );
    m_ReductionPatterns.push_back(
        std::make_tuple<std::vector<NodeType>, NodeType, RuleType>(
            {NodeType_Id, NodeType_ParenthesesOpen, NodeType_Expression, NodeType_ParenthesesClose},
            NodeType_Function,
            RuleType_Function_IdParenthesesBlock
        )
    );

    // Expression
    m_ReductionPatterns.push_back(
        std::make_tuple<std::vector<NodeType>, NodeType, RuleType>(
            {NodeType_Id},
            NodeType_Expression,
            RuleType_Expression_Id
        )
    );
    m_ReductionPatterns.push_back(
        std::make_tuple<std::vector<NodeType>, NodeType, RuleType>(
            {NodeType_Literal},
            NodeType_Expression,
            RuleType_Expression_Literal
        )
    );
    m_ReductionPatterns.push_back(
        std::make_tuple<std::vector<NodeType>, NodeType, RuleType>(
            {NodeType_Function},
            NodeType_Expression,
            RuleType_Expression_Function
        )
    );
    m_ReductionPatterns.push_back(
        std::make_tuple<std::vector<NodeType>, NodeType, RuleType>(
            {NodeType_Expression, NodeType_OperatorPlus, NodeType_Expression},
            NodeType_Expression,
            RuleType_Expression_ExpressionOpExpression
        )
    );
    m_ReductionPatterns.push_back(
        std::make_tuple<std::vector<NodeType>, NodeType, RuleType>(
            {NodeType_Expression, NodeType_OperatorMinus, NodeType_Expression},
            NodeType_Expression,
            RuleType_Expression_ExpressionOpExpression
        )
    );
    m_ReductionPatterns.push_back(
        std::make_tuple<std::vector<NodeType>, NodeType, RuleType>(
            {NodeType_Expression, NodeType_OperatorMultiply, NodeType_Expression},
            NodeType_Expression,
            RuleType_Expression_ExpressionOpExpression
        )
    );
    m_ReductionPatterns.push_back(
        std::make_tuple<std::vector<NodeType>, NodeType, RuleType>(
            {NodeType_Expression, NodeType_OperatorDivide, NodeType_Expression},
            NodeType_Expression,
            RuleType_Expression_ExpressionOpExpression
        )
    );
    m_ReductionPatterns.push_back(
        std::make_tuple<std::vector<NodeType>, NodeType, RuleType>(
            {NodeType_Expression, NodeType_OperatorEquals, NodeType_Expression},
            NodeType_Expression,
            RuleType_Expression_ExpressionOpExpression
        )
    );
    m_ReductionPatterns.push_back(
        std::make_tuple<std::vector<NodeType>, NodeType, RuleType>(
            {NodeType_Expression, NodeType_OperatorNotEquals, NodeType_Expression},
            NodeType_Expression,
            RuleType_Expression_ExpressionOpExpression
        )
    );
    m_ReductionPatterns.push_back(
        std::make_tuple<std::vector<NodeType>, NodeType, RuleType>(
            {NodeType_Expression, NodeType_OperatorLess, NodeType_Expression},
            NodeType_Expression,
            RuleType_Expression_ExpressionOpExpression
        )
    );
    m_ReductionPatterns.push_back(
        std::make_tuple<std::vector<NodeType>, NodeType, RuleType>(
            {NodeType_Expression, NodeType_OperatorLessOrEquals, NodeType_Expression},
            NodeType_Expression,
            RuleType_Expression_ExpressionOpExpression
        )
    );
    m_ReductionPatterns.push_back(
        std::make_tuple<std::vector<NodeType>, NodeType, RuleType>(
            {NodeType_Expression, NodeType_OperatorGreater, NodeType_Expression},
            NodeType_Expression,
            RuleType_Expression_ExpressionOpExpression
        )
    );
    m_ReductionPatterns.push_back(
        std::make_tuple<std::vector<NodeType>, NodeType, RuleType>(
            {NodeType_Expression, NodeType_OperatorGreaterOrEquals, NodeType_Expression},
            NodeType_Expression,
            RuleType_Expression_ExpressionOpExpression
        )
    );
    m_ReductionPatterns.push_back(
        std::make_tuple<std::vector<NodeType>, NodeType, RuleType>(
            {NodeType_ParenthesesOpen, NodeType_Expression, NodeType_ParenthesesClose},
            NodeType_Expression,
            RuleType_Expression_ParenthesesExpression
        )
    );


    // ExpressionList
    m_ReductionPatterns.push_back(
        std::make_tuple<std::vector<NodeType>, NodeType, RuleType>(
            {NodeType_ExpressionList, NodeType_Comma, NodeType_Expression},
            NodeType_ExpressionList,
            RuleType_ExpressionList_ExpressionListExpression
        )
    );
    // Must go second, so that <ExpressionList><ExpressionList> won't happen
    m_ReductionPatterns.push_back(
        std::make_tuple<std::vector<NodeType>, NodeType, RuleType>(
            {NodeType_Expression, NodeType_Comma, NodeType_Expression},
            NodeType_ExpressionList,
            RuleType_ExpressionList_Expression
        )
    );


    // Statement
    m_ReductionPatterns.push_back(
        std::make_tuple<std::vector<NodeType>, NodeType, RuleType>(
            {NodeType_Function, NodeType_Semicolon},
            NodeType_Statement,
            RuleType_Statement_Function
        )
    );
    m_ReductionPatterns.push_back(
        std::make_tuple<std::vector<NodeType>, NodeType, RuleType>(
            {NodeType_Id, NodeType_OperatorAssign, NodeType_Expression, NodeType_Semicolon},
            NodeType_Statement,
            RuleType_Statement_IdEqExpression
        )
    );
    // Must go after the previous ones, so that an empty statement(single semicolon) is tried to be matched last
    m_ReductionPatterns.push_back(
        std::make_tuple<std::vector<NodeType>, NodeType, RuleType>(
            {NodeType_Semicolon},
            NodeType_Statement,
            RuleType_Statement_Semicolon
        )
    );
    m_ReductionPatterns.push_back(
        std::make_tuple<std::vector<NodeType>, NodeType, RuleType>(
            {NodeType_BracesOpen, NodeType_BracesClose},
            NodeType_Statement,
            RuleType_Statement_Braces
        )
    );
    m_ReductionPatterns.push_back(
        std::make_tuple<std::vector<NodeType>, NodeType, RuleType>(
            {NodeType_BracesOpen, NodeType_StatementList, NodeType_BracesClose},
            NodeType_Statement,
            RuleType_Statement_BracesBlock
        )
    );
    m_ReductionPatterns.push_back(
        std::make_tuple<std::vector<NodeType>, NodeType, RuleType>(
            {NodeType_BracesOpen, NodeType_Statement, NodeType_BracesClose},
            NodeType_Statement,
            RuleType_Statement_BracesBlock
        )
    );
    m_ReductionPatterns.push_back(
        std::make_tuple<std::vector<NodeType>, NodeType, RuleType>(
            {NodeType_KeywordIf, NodeType_ParenthesesOpen, NodeType_Expression,
                NodeType_ParenthesesClose, NodeType_Statement,
                NodeType_KeywordElse, NodeType_Statement},
            NodeType_Statement,
            RuleType_Statement_IfElse
        )
    );
    m_ReductionPatterns.push_back(
        std::make_tuple<std::vector<NodeType>, NodeType, RuleType>(
            {NodeType_KeywordIf, NodeType_ParenthesesOpen, NodeType_Expression,
                NodeType_ParenthesesClose, NodeType_Statement},
            NodeType_Statement,
            RuleType_Statement_If
        )
    );


    // StatementList
    m_ReductionPatterns.push_back(
        std::make_tuple<std::vector<NodeType>, NodeType, RuleType>(
            {NodeType_StatementList, NodeType_Statement},
            NodeType_StatementList,
            RuleType_StatementList_StatementListStatement
        )
    );
    // Must go second, so that <StatementList><StatementList> won't happen
    m_ReductionPatterns.push_back(
        std::make_tuple<std::vector<NodeType>, NodeType, RuleType>(
            {NodeType_Statement, NodeType_Statement},
            NodeType_StatementList,
            RuleType_StatementList_Statement
        )
    );


    // for (Token token : tokensTable) {
    //     std::cout << token << " " << std::endl;
    // }

    // m_Nodes.push_back(new Node(NodeType_KeywordIf, "", std::vector<Node*>()));
    // m_Nodes.push_back(new Node(NodeType_ParenthesesOpen, "", std::vector<Node*>()));
    // m_Nodes.push_back(new Node(NodeType_ExpressionList, "", std::vector<Node*>()));
    // m_Nodes.push_back(new Node(NodeType_Comma, "", std::vector<Node*>()));
    // m_Nodes.push_back(new Node(NodeType_Expression, "", std::vector<Node*>()));
    // // m_Nodes.push_back(new Node(NodeType_ParenthesesClose, "", std::vector<Node*>()));
    // for (ReductionPattern rp : getPotentialReductionPatterns()) {
    //     std::cout << rp.second << " ::= ";
    //     for (NodeType nt : rp.first) {
    //         std::cout <<  nt << " ";
    //     }
    //     std::cout << std::endl;
    // }

}


const Node* SyntaxAnalyzer::getNext() {
    const Node* node = peekNext();
    if (node) {
        m_NextTokensTableIndex++;
    }

    return node;
}

const Node* SyntaxAnalyzer::peekNext() const {
    if (m_NextTokensTableIndex == m_ConvertedTokensTable.size()) {
        return nullptr;
    }

    return &m_ConvertedTokensTable[m_NextTokensTableIndex];
}


std::vector<ReductionPattern> SyntaxAnalyzer::getPotentialReductionPatterns() const {
    std::vector<ReductionPattern> potentialReductionPatterns;
    for (ReductionPattern reductionPattern : m_ReductionPatterns) {
        const std::vector<NodeType> sequence = std::get<0>(reductionPattern);
        const unsigned int sequenceLength = sequence.size();
        if (m_Nodes.size() < sequenceLength) { // potential pattern is too long
            continue;
        }

        bool mismatchFound = false;
        unsigned int reductionPatternIndex = 0;
        for (unsigned int i = m_Nodes.size() - sequenceLength;
                i < m_Nodes.size(); i++, reductionPatternIndex++) {
            const NodeType nodeTypeInReductionPattern =
                sequence.at(reductionPatternIndex);
            if (m_Nodes.at(i)->m_NodeType != nodeTypeInReductionPattern) {
                mismatchFound = true;
                break;
            }
        }

        if (!mismatchFound) {
            potentialReductionPatterns.push_back(reductionPattern);
        }
    }

    return potentialReductionPatterns;
}

bool SyntaxAnalyzer::reduceIfCan() {
    const std::vector<ReductionPattern> potentialReductionPatterns = getPotentialReductionPatterns();
    if (potentialReductionPatterns.size() == 0) {
        return false;
    }

    const NodeType currentNodeType = m_Nodes[m_Nodes.size() - 1]->m_NodeType;
    const Node* const nextNode = peekNext();

    // std::cout << "Could reduce into: ";
    // for (ReductionPattern rp : potentialReductionPatterns) {
    //     std::cout << std::get<1>(rp) << " or ";
    // }
    // std::cout << std::endl;

    if (currentNodeType == NodeType_Id && nextNode) {
        const NodeType nextNodeType = nextNode->m_NodeType;
        if (nextNodeType == NodeType_ParenthesesOpen || nextNodeType == NodeType_OperatorAssign) {
            // don't allow Id to become an expression if it is an assignment or a function
            return false;
        }
    }

    const ReductionPattern firstReductionPattern = potentialReductionPatterns.at(0);
    if (std::get<1>(firstReductionPattern) == NodeType_Expression) {
        if (currentNodeType == NodeType_Function) {
            const int indexOfPreviousNode = m_Nodes.size() - 2;
            if (indexOfPreviousNode < 0) {
                return false;
            }

            const NodeType previousNodeType = m_Nodes[indexOfPreviousNode]->m_NodeType;
            bool leftSideHasOperator = false;
            switch (previousNodeType) {
                case NodeType_OperatorAssign:
                case NodeType_OperatorPlus:
                case NodeType_OperatorMinus:
                case NodeType_OperatorMultiply:
                case NodeType_OperatorDivide:
                case NodeType_OperatorEquals:
                case NodeType_OperatorNotEquals:
                case NodeType_OperatorLess:
                case NodeType_OperatorLessOrEquals:
                case NodeType_OperatorGreater:
                case NodeType_OperatorGreaterOrEquals:
                    leftSideHasOperator = true;
                    break;
                default:
                    leftSideHasOperator = false;
            }

            bool rightSideHasOperator = true;
            if (nextNode) {
                const NodeType nextNodeType = nextNode->m_NodeType;
                switch (nextNodeType) {
                    case NodeType_OperatorPlus:
                    case NodeType_OperatorMinus:
                    case NodeType_OperatorMultiply:
                    case NodeType_OperatorDivide:
                    case NodeType_OperatorEquals:
                    case NodeType_OperatorNotEquals:
                    case NodeType_OperatorLess:
                    case NodeType_OperatorLessOrEquals:
                    case NodeType_OperatorGreater:
                    case NodeType_OperatorGreaterOrEquals:
                        rightSideHasOperator = true;
                        break;
                    default:
                        rightSideHasOperator = false;
                }
            }

            if (!leftSideHasOperator && !rightSideHasOperator) {
                // Then it can't be an expression
                return false;
            }
        }

        const int indexOfPotentialIf =
            m_Nodes.size() - std::get<0>(firstReductionPattern).size() - 1;
        if (indexOfPotentialIf >= 0) {
            // If the Node before the beginning of the pattern is IF =>
            // => don't allow (EXPR) to fold into an EXPR (because be need () for the if condition).
            if (m_Nodes.at(indexOfPotentialIf)->m_NodeType == NodeType_KeywordIf) {
                return false;
            }
        }
    }

    // If it wants to reduce onto IF => allow only if there is no ELSE(otherwise force IF_ELSE)
    if (std::get<2>(firstReductionPattern) == RuleType_Statement_If && nextNode) {
        const NodeType nextNodeType = nextNode->m_NodeType;
        if (nextNodeType == NodeType_KeywordElse) {
            return false;
        }
    }

    // Alles gut => pick the best(first is best) pattern
    // std::cout << "Doing it" << std::endl;

    // std::cout << ":> Before reduction: ";
    // for (const Node* node : m_Nodes) {
    //     std::cout << node->m_NodeType << " ";
    // }
    // std::cout << std::endl;

    const unsigned int patternSize = std::get<0>(firstReductionPattern).size();
    const unsigned int startIndex = m_Nodes.size() - patternSize;
    // std::cout << "par size: " << patternSize << " ; st index: " << startIndex << std::endl;
    std::vector<const Node*> children;
    // std::cout << "Children: ";
    for (unsigned int i = startIndex; i < startIndex + patternSize; i++) {
        children.push_back(m_Nodes[i]);
        // std::cout << m_Nodes[i]->m_NodeType << " ";
    }
    // std::cout << std::endl;

    // Remove the reduced part
    m_Nodes.erase(m_Nodes.begin() + startIndex, m_Nodes.end());

    // Insert a new Node instead of the reduced part
    m_Nodes.push_back(new Node(
        std::get<1>(firstReductionPattern),
        std::get<2>(firstReductionPattern),
        children
    ));

    std::cout << ":> After reduction: ";
    for (const Node* node : m_Nodes) {
        std::cout << node->m_NodeType << " ";
    }
    std::cout << std::endl;

    return true;
}

bool SyntaxAnalyzer::parse() {
    // Node* nodePrevious = nullptr;
    while (peekNext()) {
        // const Node* nodePrevious =
        //     m_Nodes.size() == 0 ? nullptr : m_Nodes[m_Nodes.size() - 1];
        const Node* nodeCurrent = getNext();
        // std::cout << "Read Token: " << nodeCurrent->m_NodeType << std::endl;
        // if (!isAllowed(
        //         nodePrevious->m_NodeType,
        //         nodeCurrent.m_NodeType,
        //         m_CurrentContext)) {
        //
        //     std::cout << "[Parse error]: Unexpected token \""
        //               << *nodePrevious << "\" after \""
        //               << nodeCurrent << "\"." << std::endl;
        // }

        m_Nodes.push_back(nodeCurrent);

        // std::cout << ":> Now have: ";
        // for (const Node* node : m_Nodes) {
        //     std::cout << node->m_NodeType << " ";
        // }
        // std::cout << std::endl;

        bool keepReducing = true;
        while(keepReducing) {
            keepReducing = reduceIfCan();
        }
    }


    std::cout << "------Parsing finished-------" << std::endl;

    if (m_Nodes.size() > 1) {
        // There is an error for sure, let's try to deduce where it is
        for (unsigned int i = 1; i < m_Nodes.size(); i++) {
            const NodeType previousNodeType = m_Nodes[i - 1]->m_NodeType;
            const NodeType currentNodeType = m_Nodes[i]->m_NodeType;

            if (!isAllowed(previousNodeType, currentNodeType)) {
                continue;
            }
        }



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
        const NodeType nodeType2) const {

    const auto entryForNodeType1 = m_AllowedSequences.find(nodeType1);
    if (entryForNodeType1 != m_AllowedSequences.end()) {
        const std::vector<NodeType> allowedSequences = m_AllowedSequences.at(nodeType1);
        for (NodeType nodeType : allowedSequences) {
            if (nodeType2 == nodeType) {
                return true;
            }
        }

        std::cout << "[Parse Error]: After " << nodeType1 << " expected one of the following:[";
        for (NodeType nodeType : allowedSequences) {
            std::cout << "'" << nodeType << "' ";
        }
        std::cout << "], but found '" << nodeType2 << "'" << std::endl;
    }

    return true;
}


Node SyntaxAnalyzer::convertIntoNodeType(const Token token) const {
    const TokenName tokenName = token.getName();
    const std::string tokenValue = token.getValue();
    switch(tokenName) {
        case TokenName_Identifier:
            return Node(NodeType_Id, tokenValue);
        case TokenName_Literal:
            return Node(NodeType_Literal, tokenValue);
        case TokenName_Keyword_If:
            return Node(NodeType_KeywordIf);
        case TokenName_Keyword_Else:
            return Node(NodeType_KeywordElse);
        case TokenName_Operator:
            if (tokenValue.compare("+") == 0) {
                return Node(NodeType_OperatorPlus);
            } else if (tokenValue.compare("-") == 0) {
                return Node(NodeType_OperatorMinus);
            } else if (tokenValue.compare("*") == 0) {
                return Node(NodeType_OperatorMultiply);
            } else if (tokenValue.compare("/") == 0) {
                return Node(NodeType_OperatorDivide);
            } else if (tokenValue.compare("==") == 0) {
                return Node(NodeType_OperatorEquals);
            } else if (tokenValue.compare("!=") == 0) {
                return Node(NodeType_OperatorNotEquals);
            } else if (tokenValue.compare("<") == 0) {
                return Node(NodeType_OperatorLess);
            } else if (tokenValue.compare("<=") == 0) {
                return Node(NodeType_OperatorLessOrEquals);
            } else if (tokenValue.compare(">") == 0) {
                return Node(NodeType_OperatorGreater);
            } else if (tokenValue.compare(">=") == 0) {
                return Node(NodeType_OperatorGreaterOrEquals);
            } else {
                std::cout << ":> Error: Unknown operator in convertIntoNodeType(): "
                    << tokenValue << std::endl;
            }
        case TokenName_Operator_Assign:
            return Node(NodeType_OperatorAssign);
        case TokenName_Brackets_Open:
            return Node(NodeType_BracketsOpen);
        case TokenName_Brackets_Close:
            return Node(NodeType_BracketsClose);
        case TokenName_Parentheses_Open:
            return Node(NodeType_ParenthesesOpen);
        case TokenName_Parentheses_Close:
            return Node(NodeType_ParenthesesClose);
        case TokenName_Braces_Open:
            return Node(NodeType_BracesOpen);
        case TokenName_Braces_Close:
            return Node(NodeType_BracesClose);
        case TokenName_Semicolon:
            return Node(NodeType_Semicolon);
        case TokenName_Comma:
            return Node(NodeType_Comma);
        case TokenName_Unknown:
        default:
            std::cout << ":> Error: Encountered Unknown Token in convertIntoNodeType():"
                << tokenValue;
    }

    return Node(NodeType_Semicolon);
}

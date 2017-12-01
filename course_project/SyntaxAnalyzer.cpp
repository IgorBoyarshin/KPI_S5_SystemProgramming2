#include "SyntaxAnalyzer.h"


SyntaxAnalyzer::SyntaxAnalyzer(
        const std::vector<Token> tokensTable) :
    m_ConvertedTokensTable(std::vector<Node>()),
    m_NextTokensTableIndex(0),
    m_IdsTable(std::map<std::string, std::vector<NodeType>>()),
    m_ReductionPatterns(readReductionPatterns("rules.txt")),
    m_AllowedSequences(readAllowedSequences("sequences.txt"))
{
    // Convert all tokens into their Node equivalents
    for (Token token : tokensTable) {
        m_ConvertedTokensTable.push_back(convertIntoNodeType(token));
        // std::cout << token << std::endl;
    }


    // const auto rp = readReductionPatterns("rules.txt");
    // for (const auto r : rp) {
    //     std::cout << std::get<1>(r) << std::endl;
    //     for (const auto t : std::get<0>(r)) {
    //         std::cout << " " << t;
    //     }
    //     std::cout << std::endl;
    // }


    // const auto rp = readAllowedSequences("sequences.txt");
    // for (const auto& x : rp) {
    //     std::cout << x.first << std::endl;
    //     for (const auto t : x.second) {
    //         std::cout << " " << t;
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
        // Can't reduce => nothing to do here
        return false;
    }

    const unsigned int currentNodeIndex = m_Nodes.size() - 1;
    const Node* const currentNode = m_Nodes[currentNodeIndex];
    const NodeType currentNodeType = currentNode->m_NodeType;
    const Node* const nextNode = peekNext();
    const ReductionPattern firstReductionPattern = potentialReductionPatterns.at(0);

    // std::cout << "Could reduce into: ";
    // for (ReductionPattern rp : potentialReductionPatterns) {
    //     std::cout << std::get<1>(rp) << " or ";
    // }
    // std::cout << std::endl;


    // Encountered an ID => put into the IDs table if this is a declaration,
    // otherwise convert into the appropriate type using the IDs table. If there
    // is no entry for this ID in the table => Error.
    if (currentNodeType == NodeType_Id) {
        // std::cout << "000000:" << currentNode->m_NodeValue << std::endl;
        const std::string& idValue = currentNode->m_NodeValue;
        const bool knownId =
            m_IdsTable.find(idValue) != m_IdsTable.end();
        const bool idDeclaration =
            std::get<1>(firstReductionPattern) == NodeType_TypedId;

        if (knownId && idDeclaration) {
            std::cerr << ":> [Parse Error]: Redeclaration of Id: '"
                      << idValue << "'"
                      << std::endl;
            return false;
        }

        if (!knownId && !idDeclaration) {
            std::cerr << ":> [Parse Error]: Usage of undeclared Id: '"
                      << idValue << "'"
                      << std::endl;
            return false;
        }

        // The index is valid, becase otherwise we'd have (!knownId && !idDeclaration)
        const NodeType previousNodeType = m_Nodes.at(currentNodeIndex - 1)->m_NodeType;
        if (idDeclaration) {
            m_IdsTable.insert(
                std::pair<std::string, std::vector<NodeType>>(
                    idValue,
                    {previousNodeType}
                )
            );

            // Now continue(will be reduced)
        } else { // knownId
            // So, this is a valid usage of an ID. We need to convert it into the
            // appropriate type(ID_*).

            // the 0th type is the return type
            const NodeType idType = m_IdsTable.at(idValue).at(0);
            switch (idType) {
                case NodeType_KeywordVoid:
                    m_Nodes[currentNodeIndex] = new Node(
                        NodeType_IdVoid,
                        idValue
                    );
                    break;
                case NodeType_KeywordBool:
                    m_Nodes[currentNodeIndex] = new Node(
                        NodeType_IdBool,
                        idValue
                    );
                    break;
                case NodeType_KeywordInt:
                    m_Nodes[currentNodeIndex] = new Node(
                        NodeType_IdInt,
                        idValue
                    );
                    break;
                case NodeType_KeywordFloat:
                    m_Nodes[currentNodeIndex] = new Node(
                        NodeType_IdFloat,
                        idValue
                    );
                    break;
                default:
                    std::cerr << ":> [Parse Error]: reduceIfCan(): Invalid Type for ID: "
                              << idType
                              << std::endl;
                    return false;
            }

            // Current reduction is done
            return true;
        }
    }


    // Don't allow an Id to become an expression if it is an assignment or a function
    if ((currentNodeType == NodeType_IdBool ||
            currentNodeType == NodeType_IdInt ||
            currentNodeType == NodeType_IdFloat)
            && nextNode) {
        const NodeType nextNodeType = nextNode->m_NodeType;
        if (nextNodeType == NodeType_ParenthesesOpen
                || nextNodeType == NodeType_OperatorAssign) {
            return false;
        }
    }


    // If the Node before the beginning of the pattern is IF =>
    // => don't allow (EXPR) to fold into an EXPR (because be need () for the if condition).
    if (std::get<1>(firstReductionPattern) == NodeType_ExpressionBool) {
        const int indexOfPotentialIf =
            currentNodeIndex - std::get<0>(firstReductionPattern).size();
        if (indexOfPotentialIf >= 0) {
            if (m_Nodes.at(indexOfPotentialIf)->m_NodeType == NodeType_KeywordIf) {
                return false;
            }
        }
    }


    // If it wants to reduce into a bool EXPR => don't allow if
    // there is an arithmetic operator coming up next.
    if (std::get<1>(firstReductionPattern) == NodeType_ExpressionBool) {
        const unsigned int indexOfPotentialOperator = m_NextTokensTableIndex + 0;
        if (indexOfPotentialOperator < m_ConvertedTokensTable.size()) {
            const NodeType operatorType =
                m_ConvertedTokensTable.at(indexOfPotentialOperator).m_NodeType;
            if (
                    operatorType == NodeType_OperatorPlus ||
                    operatorType == NodeType_OperatorMinus ||
                    operatorType == NodeType_OperatorMultiply ||
                    operatorType == NodeType_OperatorDivide ||
                    operatorType == NodeType_OperatorLess ||
                    operatorType == NodeType_OperatorLessOrEquals ||
                    operatorType == NodeType_OperatorGreater ||
                    operatorType == NodeType_OperatorGreaterOrEquals
                    ) {
                return false;
            }
        }
    }


    // TODO: implement
    // If it wants to reduce the (+) or (-) operator => don't allow if there
    // is an (*) or (/) operator coming up next
    // if (std::get<0>(firstReductionPattern).size() > 0) {
    //     if (std::get<0>(firstReductionPattern).at(1) == NodeType_OperatorPlus ||
    //             std::get<0>(firstReductionPattern).at(1) == NodeType_OperatorMinus) {
    //         const unsigned int indexOfPotentialMoreSignificantOperator = m_NextTokensTableIndex;
    //         if (indexOfPotentialMoreSignificantOperator < m_ConvertedTokensTable.size()) {
    //             const NodeType operatorType =
    //                 m_ConvertedTokensTable.at(indexOfPotentialMoreSignificantOperator).m_NodeType;
    //             if (
    //                     operatorType == NodeType_OperatorMultiply ||
    //                     operatorType == NodeType_OperatorDivide
    //                     ) {
    //                 return false;
    //             }
    //         }
    //     }
    // }

    // If it wants to reduce onto IF => allow only if there is no
    // following ELSE(otherwise force IF_ELSE). The size of the IF pattern is 5
    if (std::get<0>(firstReductionPattern).at(0) == NodeType_KeywordIf
            && std::get<0>(firstReductionPattern).size() == 5 && nextNode) {
        const NodeType nextNodeType = nextNode->m_NodeType;
        if (nextNodeType == NodeType_KeywordElse) {
            return false;
        }
    }

    // Alles gut => pick the best(first is best) pattern

    // std::cout << ":> Before reduction: ";
    // for (const Node* node : m_Nodes) {
    //     std::cout << node->m_NodeType << " ";
    // }
    // std::cout << std::endl;
    //

    const unsigned int patternSize = std::get<0>(firstReductionPattern).size();
    const unsigned int startIndex = m_Nodes.size() - patternSize;
    std::vector<const Node*> children;
    for (unsigned int i = startIndex; i < startIndex + patternSize; i++) {
        children.push_back(m_Nodes[i]);
    }

    // Remove the reduced part
    m_Nodes.erase(m_Nodes.begin() + startIndex, m_Nodes.end());

    // Insert a new Node instead of the reduced part
    m_Nodes.push_back(new Node(
        std::get<1>(firstReductionPattern),
        children
    ));

    // std::cout << ":> After reduction: ";
    // for (const Node* node : m_Nodes) {
    //     std::cout << node->m_NodeType << " ";
    // }
    // std::cout << std::endl;
    //

    return true;
}

bool SyntaxAnalyzer::parse() {
    while (peekNext()) {
        const Node* nodeCurrent = getNext();
        m_Nodes.push_back(nodeCurrent);
        bool keepReducing = true;
        while(keepReducing) {
            keepReducing = reduceIfCan();
        }
    }

    // std::cout << ":> Parsed tree:" << std::endl;
    // std::cout << *m_Nodes[0] << std::endl;

    if (m_Nodes.size() > 1) {
        // There is an error for sure, let's try to deduce where it is
        for (unsigned int i = 1; i < m_Nodes.size(); i++) {
            const NodeType previousNodeType = m_Nodes[i - 1]->m_NodeType;
            const NodeType currentNodeType = m_Nodes[i]->m_NodeType;

            if (!isAllowed(previousNodeType, currentNodeType)) {
                continue;
            }
        }

        std::cout << ":> [Parse Error]: Unexpected input end." << std::endl;
        return false;
    }

    const auto topLevelNodeType = m_Nodes[0]->m_NodeType;
    if (!(topLevelNodeType == NodeType_DeclarationList ||
            topLevelNodeType == NodeType_Declaration)) {
        std::cout << ":> [Parse Error]: Unexpected top-level entry." << std::endl;
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

        std::cout << ":> [Parse Error]: After " << nodeType1 << " expected one of the following:[";
        for (NodeType nodeType : allowedSequences) {
            std::cout << "'" << nodeType << "' ";
        }
        std::cout << "], but found '" << nodeType2 << "'" << std::endl;
    }

    return true;
}


Node SyntaxAnalyzer::convertIntoNodeType(const Token token) const {
    const TokenType tokenType = token.getType();
    const std::string tokenValue = token.getValue();
    switch(tokenType) {
        case TokenType_Identifier:
            return Node(NodeType_Id, tokenValue);
        case TokenType_LiteralBool:
            return Node(NodeType_LiteralBool, tokenValue);
        case TokenType_LiteralInt:
            return Node(NodeType_LiteralInt, tokenValue);
        case TokenType_LiteralFloat:
            return Node(NodeType_LiteralFloat, tokenValue);
        case TokenType_Keyword_If:
            return Node(NodeType_KeywordIf);
        case TokenType_Keyword_Else:
            return Node(NodeType_KeywordElse);
        case TokenType_Keyword_Void:
            return Node(NodeType_KeywordVoid);
        case TokenType_Keyword_Bool:
            return Node(NodeType_KeywordBool);
        case TokenType_Keyword_Int:
            return Node(NodeType_KeywordInt);
        case TokenType_Keyword_Float:
            return Node(NodeType_KeywordFloat);
        case TokenType_Operator:
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
        case TokenType_Operator_Assign:
            return Node(NodeType_OperatorAssign);
        case TokenType_Brackets_Open:
            return Node(NodeType_BracketsOpen);
        case TokenType_Brackets_Close:
            return Node(NodeType_BracketsClose);
        case TokenType_Parentheses_Open:
            return Node(NodeType_ParenthesesOpen);
        case TokenType_Parentheses_Close:
            return Node(NodeType_ParenthesesClose);
        case TokenType_Braces_Open:
            return Node(NodeType_BracesOpen);
        case TokenType_Braces_Close:
            return Node(NodeType_BracesClose);
        case TokenType_Semicolon:
            return Node(NodeType_Semicolon);
        case TokenType_Comma:
            return Node(NodeType_Comma);
        case TokenType_Unknown:
        default:
            std::cout << ":> Error: Encountered Unknown Token in convertIntoNodeType():"
                << tokenValue;
    }

    return Node(NodeType_Semicolon);
}


template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}


std::vector<ReductionPattern> readReductionPatterns(const std::string& fileName) {
    std::ifstream in(fileName);
    std::vector<ReductionPattern> reductionPatterns;
    if (!in.is_open()) {
        std::cerr << ":> Unable to open: " << fileName << std::endl;
        return reductionPatterns;
    }

    std::string line;
    bool insideRule = false;
    std::vector<NodeType> from;
    NodeType to;
    while (std::getline(in, line)) {
        if (line.size() == 0) {
            insideRule = false;
            continue;
        }

        // Skip comments
        if (line[0] == '*') {
            continue;
        }

        const std::vector<std::string> words = split(line, ' ');
        if (insideRule) {
            for (const std::string& word : words) {
                if (word.size() == 0) {
                    continue;
                }

                // std::cout << "Read line " << lineCounter << std::endl;
                from.push_back(getNodeTypeFromString(word));
            }

            reductionPatterns.push_back(
                std::make_tuple<std::vector<NodeType>, NodeType>(
                    std::move(from),
                    std::move(to)
                )
            );

            from.clear();
        } else {
            // Prepare to read rules
            insideRule = true;
            to = getNodeTypeFromString(words[0]);
        }
    }

    in.close();

    return reductionPatterns;
}


std::map<NodeType, std::vector<NodeType>> readAllowedSequences(const std::string& fileName) {
    std::ifstream in(fileName);
    std::map<NodeType, std::vector<NodeType>> allowedSequences;
    if (!in.is_open()) {
        std::cerr << ":> Unable to open: " << fileName << std::endl;
        return allowedSequences;
    }

    std::string line;
    bool insideRule = false;
    std::vector<NodeType> to;
    NodeType from;
    while (std::getline(in, line)) {
        if (line.size() == 0) {
            insideRule = false;
            continue;
        }

        // Skip comments
        if (line[0] == '%') {
            continue;
        }

        const std::vector<std::string> words = split(line, ' ');
        if (insideRule) {
            for (const std::string& word : words) {
                if (word.size() == 0) {
                    continue;
                }

                to.push_back(getNodeTypeFromString(word));
            }

            allowedSequences.insert(
                std::pair<NodeType, std::vector<NodeType>>(
                    std::move(from),
                    std::move(to)
                )
            );

            to.clear();
        } else {
            // Prepare to read rules
            insideRule = true;
            from = getNodeTypeFromString(words[0]);
        }
    }

    in.close();

    return allowedSequences;
}


const Node* SyntaxAnalyzer::getRootNode() const {
    return m_Nodes[0];
}

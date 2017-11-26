#include "CodeGenerator.h"


CodeGenerator::CodeGenerator(const Node* rootNode)
    : m_RootNode(rootNode),
    m_Variables(getAllVariableDeclarations(rootNode)),
    m_Functions(getAllFunctionDeclarations(rootNode)) {
        // for (const auto& pair : m_Variables) {
        //     std::cout << "Var: " << pair.second << " " << pair.first << std::endl;
        // }
        //

        // for (const auto& function : m_Functions) {
        //     std::cout << "Func: " << function.second[0].second << " "
        //               << function.second[0].first << ": ";
        //     for (unsigned int i = 1; i < function.second.size(); i++) {
        //         std::cout << " " << function.second[i].second << " "
        //                   << function.second[i].first << ", ";
        //     }
        //     std::cout << std::endl;
        // }
    }


std::string CodeGenerator::generate() {
    // We know that the top-level entry is either DECLARATION or DECLARATION_LIST.
    // Search among them for function declarations and implement their bodies.

    std::stringstream code;
    code << ".386" << std::endl;
    code << ".model flat" << std::endl;
    code << ".stack 4096" << std::endl;
    code << std::endl;

    code << ".data" << std::endl;
    generateVariableDeclarationsCode(code);
    code << std::endl;

    code << ".code" << std::endl;
    code << std::endl;

    // Will generate function declarations
    // (which is essentially all there is left to do)
    generateDeclarationCode(code, m_RootNode);

    return code.str();
}


void CodeGenerator::generateDeclarationCode(
        std::stringstream& code,
        const Node* node) const {

    if (
            node->m_NodeType != NodeType_Declaration &&
            node->m_NodeType != NodeType_DeclarationList) {
        return;
    }

    if (node->m_NodeType == NodeType_DeclarationList) {
        generateDeclarationCode(code, node->m_Children[0]);
        generateDeclarationCode(code, node->m_Children[1]);
        return;
    }

    // The type is Declaration for sure
    // The next function will return safely even if it is called upon not a function declaration
    generateFunctionDeclarationCode(code, node);
}


std::vector< std::pair<std::string, NodeType> >
    CodeGenerator::getAllVariableDeclarations(const Node* parentNode) const {

    std::vector< std::pair<std::string, NodeType> > declarations;

    const std::vector<const Node*> children = parentNode->m_Children;
    for (unsigned int index = 0; index < children.size(); index++) {
        const Node* child = children[index];

        if (
                child->m_NodeType == NodeType_Declaration ||
                child->m_NodeType == NodeType_DeclarationList ||
                child->m_NodeType == NodeType_TypedIdList
           ) {
            const std::vector< std::pair<std::string, NodeType> >
                childDeclarations = getAllVariableDeclarations(child);
            declarations.insert(
                    declarations.end(),
                    childDeclarations.begin(),
                    childDeclarations.end()
            );
        } else if (child->m_NodeType == NodeType_TypedId) {
            const bool functionId = (index + 1) < children.size() &&
                children[index + 1]->m_NodeType == NodeType_ParenthesesOpen;
            // std::cout << "Found: " << typedIdChildren[0]->m_NodeType << " "
                // << typedIdChildren[1]->m_NodeValue << ", func: " << functionId << std::endl;
            if (!functionId) {
                const std::vector<const Node*> typedIdChildren = child->m_Children;
                const NodeType type = typedIdChildren[0]->m_NodeType;
                const std::string name = typedIdChildren[1]->m_NodeValue;
                declarations.push_back(std::pair<std::string, NodeType>(name, type));
            }
        }
    }

    return declarations;
}

std::map< std::string, std::vector< std::pair<std::string, NodeType> > >
    CodeGenerator::getAllFunctionDeclarations(const Node* parentNode) const {

    std::map< std::string, std::vector< std::pair<std::string, NodeType> > > declarations;

    const std::vector<const Node*> children = parentNode->m_Children;
    for (unsigned int index = 0; index < children.size(); index++) {
        const Node* child = children[index];

        if (
                child->m_NodeType == NodeType_Declaration ||
                child->m_NodeType == NodeType_DeclarationList ||
                child->m_NodeType == NodeType_TypedIdList
           ) {
            std::map< std::string, std::vector< std::pair<std::string, NodeType> > >
                childDeclarations = getAllFunctionDeclarations(child);
            declarations.insert(
                    childDeclarations.begin(),
                    childDeclarations.end()
            );
        } else if (child->m_NodeType == NodeType_TypedId) {
            const bool functionId = (index + 1) < children.size() &&
                children[index + 1]->m_NodeType == NodeType_ParenthesesOpen;
            if (functionId) {
                const std::vector<const Node*> typedIdChildren = child->m_Children;
                const NodeType functionType = typedIdChildren[0]->m_NodeType;
                const std::string functionName = typedIdChildren[1]->m_NodeValue;
                // declarations.push_back(std::pair<std::string, NodeType>(name, type));
                const std::vector<std::pair<std::string, NodeType>> variableDeclarations =
                    getAllVariableDeclarations(parentNode);
                std::vector< std::pair<std::string, NodeType> > arguments;
                arguments.push_back(std::pair<std::string, NodeType>(functionName, functionType)); // the 0th type is the function's return type
                for (const auto& argument : variableDeclarations) {
                    arguments.push_back(std::pair<std::string, NodeType>(
                        argument.first, argument.second
                    ));
                }
                declarations.insert(
                    std::pair<std::string, std::vector< std::pair<std::string, NodeType>> >(
                        functionName, arguments
                ));
            }
        }
    }

    return declarations;
}


void CodeGenerator::generateVariableDeclarationsCode(std::stringstream& code) const {
    for (const auto& var : m_Variables) {
        code << "    " << var.first << " " << getAssemblyTypeFor(var.second)
             << " " << getAssemblyZeroFor(var.second) << std::endl;
    }
}


void CodeGenerator::generateBlockCode(
    std::stringstream& code,
    const Node* blockNode) const {

    if (blockNode->m_NodeType != NodeType_Block) {
        return;
    }

    // A Node is either {} or {STMT} or {STMT_LIST}

    if (blockNode->m_Children.size() == 2) {
        // The {} case.
        // No statements => no code.
        // No money => no honey.
        return;
    }

    const Node* blockChild = blockNode->m_Children[1];
    if (blockChild->m_NodeType == NodeType_StatementList) {
        generateStatementListCode(code, blockChild);
    } else if (blockChild->m_NodeType == NodeType_Statement) {
        generateStatementCode(code, blockChild);
    }
}


void CodeGenerator::generateStatementListCode(
    std::stringstream& code,
    const Node* statementListNode) const {

    if (statementListNode->m_NodeType != NodeType_StatementList) {
        return;
    }

    const std::vector<const Node*> children =
        statementListNode->m_Children;

    if (children[0]->m_NodeType == NodeType_StatementList) {
        generateStatementListCode(code, children[0]);
    } else if (children[0]->m_NodeType == NodeType_Statement) {
        generateStatementCode(code, children[0]);
    }

    generateStatementCode(code, children[1]);
}


void CodeGenerator::generateStatementCode(
    std::stringstream& code,
    const Node* statementNode) const {

    if (statementNode->m_NodeType != NodeType_Statement) {
        return;
    }

    const std::vector<const Node*> children = statementNode->m_Children;
    const NodeType firstNodeType = children[0]->m_NodeType;
    switch (firstNodeType) {
        case NodeType_Block:
            generateBlockCode(code, children[0]);
            break;
        case NodeType_FunctionCallVoid:
            // TODO
            break;
        case NodeType_IdInt: {
            const Node* expressionRoot = children[2];
            generateExpressionEvaluation(code, expressionRoot);
            code << std::endl;

            // The previous function will have stored the result of the
            // expression in the EAX register
            // code << shift() << "lea edi, " << children[0]->m_NodeValue << std::endl
            //      << shift() << "mov dword ptr[edi], eax" << std::endl
            code << shift() << "mov " << children[0]->m_NodeValue << ", eax" << std::endl
                 << std::endl;
            break;
        }
        default:
            break;
    }
}


// The result of evaluation is always stored in the EAX register
void CodeGenerator::generateExpressionEvaluation(
    std::stringstream& code,
    const Node* expressionRoot) const {

    const NodeType expressionType = expressionRoot->m_NodeType;
    const std::vector<const Node*> children = expressionRoot->m_Children;
    const Node* firstNode = children[0];
    const NodeType firstNodeType = firstNode->m_NodeType;
    switch (expressionType) {
        case NodeType_ExpressionFloat:
        case NodeType_ExpressionInt: {
            switch (firstNodeType) {
                case NodeType_IdFloat:
                case NodeType_IdInt: {
                    // code << shift() << "lea esi, "
                    //                 << firstNode->m_NodeValue << "" << std::endl
                    code << shift() << "mov eax, " << firstNode->m_NodeValue
                                    << "" << std::endl;
                    // code << std::endl;
                    break;
                }
                case NodeType_LiteralFloat:
                case NodeType_LiteralInt: {
                    code << shift() << "mov eax, " << firstNode->m_NodeValue << std::endl;
                    // code << std::endl;
                    break;
                }
                case NodeType_FunctionCallFloat:
                case NodeType_FunctionCallInt:
                    generateFunctionCallCode(code, firstNode);
                    break;
                case NodeType_ParenthesesOpen:
                    generateExpressionEvaluation(code, children[1]);
                    break;
                case NodeType_ExpressionFloat:
                case NodeType_ExpressionInt: { // arithmetic operations
                    const Node* operand1 = children[0];
                    const Node* operand2 = children[2];
                    const NodeType operation = children[1]->m_NodeType;

                    generateExpressionEvaluation(code, operand2);
                    code << shift() << "mov ebx, eax" << std::endl;
                         // << std::endl;
                    generateExpressionEvaluation(code, operand1);

                    switch (operation) {
                        case NodeType_OperatorPlus:
                            code << shift() << "add eax, ebx" << std::endl;
                            break;
                        case NodeType_OperatorMinus:
                            code << shift() << "sub eax, ebx" << std::endl;
                            break;
                        case NodeType_OperatorMultiply:
                            code << shift() << "mul ebx" << std::endl
                                 << shift() << "mov eax, edx" << std::endl;
                            break;
                        case NodeType_OperatorDivide:
                            code << shift() << "xor edx, edx" << std::endl
                                 << shift() << "div ebx" << std::endl;
                            break;
                        default:
                            break;
                    }
                    // code << std::endl;

                    break;
                }
                default:
                    break;
            }
            break;
        }
        default:
            break;
    }
}


// In order to call a function we need to evaluate all arguments(expressions)
// and push them, then call the function.
void CodeGenerator::generateFunctionCallCode(
    std::stringstream& code,
    const Node* functionNode) const {

    const std::vector<const Node*> children = functionNode->m_Children;
    const NodeType functionType = functionNode->m_NodeType;
    const std::string functionName = children[0]->m_NodeValue;
    // const NodeType functionReturnType = children[0]->m_NodeType;
    switch (functionType) {
        case NodeType_FunctionCallInt:
        case NodeType_FunctionCallFloat:
        case NodeType_FunctionCallBool:
        case NodeType_FunctionCallVoid: {
            const bool argumentsPresent = children.size() > 3;
            // const Node* expressionListRoot = children[]
            if (argumentsPresent) {
                generateExpressionListPush(code, children[2]);
            }
            code << shift() << "call " << functionName << std::endl;
            code << std::endl;
            break;
        }
        default:
            break;
    }
}


// Handles individual Expressions as well
int CodeGenerator::generateExpressionListPush(
    std::stringstream& code,
    const Node* expressionListRoot) const {
    
    unsigned int pushedExpressions = 0;
    const NodeType expressionListRootType = expressionListRoot->m_NodeType;
    if (
            expressionListRootType == NodeType_ExpressionBool ||
            expressionListRootType == NodeType_ExpressionInt ||
            expressionListRootType == NodeType_ExpressionFloat
        ) {
        generateExpressionEvaluation(code, expressionListRoot);
        code << shift() << "push eax" << std::endl;
        pushedExpressions++;
    } else if (expressionListRootType == NodeType_ExpressionList) {
        const Node* child1 = expressionListRoot->m_Children[0];
        const Node* child2 = expressionListRoot->m_Children[2];
        pushedExpressions += generateExpressionListPush(code, child1);
        pushedExpressions += generateExpressionListPush(code, child2);
    }

    return pushedExpressions;
}


// Will safely return if it is not a function declaration
void CodeGenerator::generateFunctionDeclarationCode(
    std::stringstream& code,
    const Node* functionDeclarationNode) const {

    if (functionDeclarationNode->m_NodeType != NodeType_Declaration) {
        return;
    }

    // The BLOCK is always the last child
    const unsigned int amountOfChildren = functionDeclarationNode->m_Children.size();
    const Node* lastChildNode = functionDeclarationNode->m_Children[amountOfChildren - 1];

    if (lastChildNode->m_NodeType != NodeType_Block) {
        // If it ain't a function declaration => no business here
        return;
    }

    // Generate function header

    const std::string functionName =
        functionDeclarationNode->m_Children[0]->m_Children[1]->m_NodeValue;
    const std::vector< std::pair<std::string, NodeType> > functionArguments =
        m_Functions.at(functionName);
    // const unsigned int amountOfArguments = functionArguments.size() - 1; // 0th is the return type
    // const NodeType functionReturnType = functionArguments[0];

    // Function title
    code << functionName << " proc" << std::endl;
    // code << shift() << "push ebp" << std::endl
    //      << shift() << "mov ebp, esp" << std::endl;
    code << std::endl;

    // Retrieve arguments from the srack
    // const unsigned int firstArgShift = (amountOfArguments + 1) * 4;
    // for (unsigned int i = 1; i < functionArguments.size(); i++) {
    //     code << shift() << "mov eax, dword ptr[ebp + " << (firstArgShift - (i - 1) * 4) << "]" << std::endl;
    //     code << shift() << "lea edi, " << functionArguments[i].first << std::endl;
    //     code << shift() << "mov dword ptr[edi], eax" << std::endl;
    //     code << std::endl;
    // }

    // Generate function body
    generateBlockCode(code, lastChildNode);


    // Generate function end
    // code << shift() << "mov esp, ebp" << std::endl
    //      << shift() << "pop ebp" << std::endl
    //      << shift() << "ret " << (amountOfArguments * 4) << std::endl;
    code << functionName << " endp" << std::endl;
    code << std::endl;
}





std::string CodeGenerator::getAssemblyTypeFor(NodeType nodeType) const {
    switch(nodeType) {
        case NodeType_KeywordInt:
            return "dd";
        case NodeType_KeywordBool:
            return "db";
        case NodeType_KeywordFloat:
            return "dq";
        default:
            return "??";
    }
}

std::string CodeGenerator::getAssemblyZeroFor(NodeType nodeType) const {
    switch(nodeType) {
        case NodeType_KeywordInt:
            return "0";
        case NodeType_KeywordBool:
            return "0";
        case NodeType_KeywordFloat:
            return "0.0";
        default:
            return "??";
    }
}


std::string CodeGenerator::shift() const {
    return "    ";
}

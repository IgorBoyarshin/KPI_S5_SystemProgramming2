#include "CodeGenerator.h"


CodeGenerator::CodeGenerator(const Node* rootNode)
    : m_RootNode(rootNode),
    m_Variables(getAllVariableDeclarations(rootNode)),
    m_Functions(getAllFunctionDeclarations(rootNode)), m_ErrorFound(false),
    m_NextAvailablyLoopIndex(0) {
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


void CodeGenerator::generateVariableDeclarationsCode(std::stringstream& code) const {
    for (const auto& var : m_Variables) {
        code << shift() << var.first << " " << getAssemblyTypeFor(var.second)
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


// TODO: if
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
            generateFunctionCallCode(code, children[0]);
            break;
        case NodeType_IdInt: { // assignment
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
        case NodeType_KeywordIf:
            if (children.size() >= 5) { //if statement OR ifelse statement
                // Generate the first part
                generateExpressionEvaluation(code, children[2]);
                code << shift() << "cmp eax, 1" << std::endl
                     << shift() << "jne loop" << m_NextAvailablyLoopIndex << std::endl;
                generateStatementCode(code, children[4]);
                code << std::endl;
                m_NextAvailablyLoopIndex++;
                code << shift() << "jmp loop" << m_NextAvailablyLoopIndex << std::endl;
            }

            code << "loop" << (m_NextAvailablyLoopIndex - 1) << ":" << std::endl;
            m_NextAvailablyLoopIndex++;

            if (children.size() == 7) { // ifelse statement
                // Generate the 'else' part if it exists(if size == 7)
                generateStatementCode(code, children[6]);
                code << "loop" << (m_NextAvailablyLoopIndex - 1) << ":" << std::endl;
                code << std::endl;
                m_NextAvailablyLoopIndex++;
            }
            break;

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
        case NodeType_ExpressionBool:
        case NodeType_ExpressionFloat:
        case NodeType_ExpressionInt: {
            switch (firstNodeType) {
                case NodeType_IdBool:
                case NodeType_IdFloat:
                case NodeType_IdInt: {
                    // code << shift() << "lea esi, "
                    //                 << firstNode->m_NodeValue << "" << std::endl
                    code << shift() << "mov eax, " << firstNode->m_NodeValue
                                    << "" << std::endl;
                    // code << std::endl;
                    break;
                }
                case NodeType_LiteralBool:
                    code << shift() << "mov eax, ";
                    if (firstNode->m_NodeValue == "true") {
                       code << "1";
                    } else if (firstNode->m_NodeValue == "false"){
                       code << "0";
                    }
                    code << std::endl;
                    break;
                case NodeType_LiteralFloat:
                case NodeType_LiteralInt: {
                    code << shift() << "mov eax, " << firstNode->m_NodeValue << std::endl;
                    // code << std::endl;
                    break;
                }
                case NodeType_FunctionCallBool:
                case NodeType_FunctionCallFloat:
                case NodeType_FunctionCallInt:
                    generateFunctionCallCode(code, firstNode);
                    break;
                case NodeType_ParenthesesOpen:
                    generateExpressionEvaluation(code, children[1]);
                    break;
                case NodeType_ExpressionBool:
                case NodeType_ExpressionFloat:
                case NodeType_ExpressionInt: { // arithmetic operations
                    const Node* operand1 = children[0];
                    const Node* operand2 = children[2];
                    const NodeType operation = children[1]->m_NodeType;

                    generateExpressionEvaluation(code, operand2);
                    code << shift() << "mov ebx, eax" << std::endl;
                         // << std::endl;
                    generateExpressionEvaluation(code, operand1);

                    // The arguments are now in EAX and EBX

                    std::string addedF(operand1->m_NodeType == NodeType_ExpressionFloat ? "f" : "");
                    switch (operation) {
                        case NodeType_OperatorPlus:
                            code << shift() << addedF << "add eax, ebx" << std::endl;
                            break;
                        case NodeType_OperatorMinus:
                            code << shift() << addedF <<  "sub eax, ebx" << std::endl;
                            break;
                        case NodeType_OperatorMultiply:
                            code << shift() << addedF << "mul ebx" << std::endl
                                 << shift() << "mov eax, edx" << std::endl;
                            break;
                        case NodeType_OperatorDivide:
                            code << shift() << "xor edx, edx" << std::endl
                                 << shift() << addedF << "div ebx" << std::endl;
                            break;

                        case NodeType_OperatorEquals:
                            code << shift() << addedF << "cmp eax, ebx" << std::endl;
                            code << shift() << "je loop" << m_NextAvailablyLoopIndex << std::endl;
                            // Not equals
                            code << shift() << "mov eax, 0" << std::endl;
                            code << shift() << "jmp loop" << (m_NextAvailablyLoopIndex + 1) << std::endl;
                            // Equals
                            code << "loop" << m_NextAvailablyLoopIndex << ":" << std::endl;
                            code << shift() << "mov eax, 1" << std::endl;
                            code << "loop" << (m_NextAvailablyLoopIndex + 1) << ":" << std::endl;
                            m_NextAvailablyLoopIndex += 2;
                            break;
                        case NodeType_OperatorNotEquals:
                            code << shift() << addedF << "cmp eax, ebx" << std::endl;
                            code << shift() << "je loop" << m_NextAvailablyLoopIndex << std::endl;
                            // Not equals
                            code << shift() << "mov eax, 1" << std::endl;
                            code << shift() << "jmp loop" << (m_NextAvailablyLoopIndex + 1) << std::endl;
                            // Equals
                            code << "loop" << m_NextAvailablyLoopIndex << ":" << std::endl;
                            code << shift() << "mov eax, 0" << std::endl;
                            code << "loop" << (m_NextAvailablyLoopIndex + 1) << ":" << std::endl;
                            m_NextAvailablyLoopIndex += 2;
                            break;
                        case NodeType_OperatorLess:
                            code << shift() << addedF << "cmp eax, ebx" << std::endl;
                            code << shift() << "jl loop" << m_NextAvailablyLoopIndex << std::endl;
                            // False
                            code << shift() << "mov eax, 0" << std::endl;
                            code << shift() << "jmp loop" << (m_NextAvailablyLoopIndex + 1) << std::endl;
                            // True
                            code << "loop" << m_NextAvailablyLoopIndex << ":" << std::endl;
                            code << shift() << "mov eax, 1" << std::endl;
                            code << "loop" << (m_NextAvailablyLoopIndex + 1) << ":" << std::endl;
                            m_NextAvailablyLoopIndex += 2;
                            break;
                        case NodeType_OperatorGreater:
                            code << shift() << addedF << "cmp eax, ebx" << std::endl;
                            code << shift() << "jg loop" << m_NextAvailablyLoopIndex << std::endl;
                            // False
                            code << shift() << "mov eax, 0" << std::endl;
                            code << shift() << "jmp loop" << (m_NextAvailablyLoopIndex + 1) << std::endl;
                            // True
                            code << "loop" << m_NextAvailablyLoopIndex << ":" << std::endl;
                            code << shift() << "mov eax, 1" << std::endl;
                            code << "loop" << (m_NextAvailablyLoopIndex + 1) << ":" << std::endl;
                            m_NextAvailablyLoopIndex += 2;
                            break;
                        case NodeType_OperatorLessOrEquals:
                            code << shift() << addedF << "cmp eax, ebx" << std::endl;
                            code << shift() << "jle loop" << m_NextAvailablyLoopIndex << std::endl;
                            // False
                            code << shift() << "mov eax, 0" << std::endl;
                            code << shift() << "jmp loop" << (m_NextAvailablyLoopIndex + 1) << std::endl;
                            // True
                            code << "loop" << m_NextAvailablyLoopIndex << ":" << std::endl;
                            code << shift() << "mov eax, 1" << std::endl;
                            code << "loop" << (m_NextAvailablyLoopIndex + 1) << ":" << std::endl;
                            m_NextAvailablyLoopIndex += 2;
                            break;
                        case NodeType_OperatorGreaterOrEquals:
                            code << shift() << addedF << "cmp eax, ebx" << std::endl;
                            code << shift() << "jge loop" << m_NextAvailablyLoopIndex << std::endl;
                            // False
                            code << shift() << "mov eax, 0" << std::endl;
                            code << shift() << "jmp loop" << (m_NextAvailablyLoopIndex + 1) << std::endl;
                            // True
                            code << "loop" << m_NextAvailablyLoopIndex << ":" << std::endl;
                            code << shift() << "mov eax, 1" << std::endl;
                            code << "loop" << (m_NextAvailablyLoopIndex + 1) << ":" << std::endl;
                            m_NextAvailablyLoopIndex += 2;
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
            const auto functionExpectedTypes = m_Functions.at(functionName);
            if (argumentsPresent) {
                const auto types = generateExpressionListPush(code, children[2]);
                if (functionExpectedTypes.size() - 1 != types.size()) {
                    m_ErrorFound = true;
                    std::cout << "[Parse Error]: specified function arguments do not match with those specified at function declaration(for function " << functionName << ")." << std::endl;
                    return;
                }

                for (unsigned int i = 0; i < types.size(); i++) {
                    if (functionExpectedTypes[i+1].second != types[i]) {
                        m_ErrorFound = true;
                        std::cout << "[Parse Error]: Specified function arguments do not match with those specified at function declaration(for function " << functionName << ")." << std::endl;
                        std::cout << "[Parse Error]: Expected:Found: " << functionExpectedTypes[i+1].second << ":" << types[i] << std::endl;
                        return;
                    }
                }
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
// Returns the list of arguments' types
std::vector<NodeType> CodeGenerator::generateExpressionListPush(
    std::stringstream& code,
    const Node* expressionListRoot) const {

    std::vector<NodeType> types;
    const NodeType expressionListRootType = expressionListRoot->m_NodeType;
    if (
            expressionListRootType == NodeType_ExpressionBool ||
            expressionListRootType == NodeType_ExpressionInt ||
            expressionListRootType == NodeType_ExpressionFloat
        ) {
        generateExpressionEvaluation(code, expressionListRoot);
        code << shift() << "push eax" << std::endl;

        if (expressionListRootType == NodeType_ExpressionBool) {
            types.push_back(NodeType_KeywordBool);
        } else if (expressionListRootType == NodeType_ExpressionInt) {
            types.push_back(NodeType_KeywordInt);
        } else if (expressionListRootType == NodeType_ExpressionFloat) {
            types.push_back(NodeType_KeywordFloat);
        }
    } else if (expressionListRootType == NodeType_ExpressionList) {
        const Node* child1 = expressionListRoot->m_Children[0];
        const Node* child2 = expressionListRoot->m_Children[2];
        const auto res1 = generateExpressionListPush(code, child1);
        const auto res2 = generateExpressionListPush(code, child2);
        types.insert(
                types.end(),
                res1.begin(),
                res1.end()
        );
        types.insert(
                types.end(),
                res2.begin(),
                res2.end()
        );
    }

    return types;
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
    // const NodeType functionReturnType = functionArguments[0];

    // Function title
    code << functionName << " proc" << std::endl;
    // code << shift() << "push ebp" << std::endl
    //      << shift() << "mov ebp, esp" << std::endl;
    code << std::endl;

    // Retrieve arguments from the srack
    const unsigned int amountOfArguments = functionArguments.size() - 1; // 0th is the return type
    const unsigned int firstArgShift = (amountOfArguments + 1) * 4;
    for (unsigned int i = 1; i < functionArguments.size(); i++) {
        code << shift() << "mov eax, dword ptr[ebp + " << (firstArgShift - (i - 1) * 4) << "]" << std::endl;
        // code << shift() << "lea edi, " << functionArguments[i].first << std::endl;
        // code << shift() << "mov dword ptr[edi], eax" << std::endl;
        code << shift() << "mov " << functionArguments[i].first << ", eax" << std::endl;
        code << std::endl;
    }

    // Generate function body
    generateBlockCode(code, lastChildNode);


    // Generate function end
    // code << shift() << "mov esp, ebp" << std::endl
    //      << shift() << "pop ebp" << std::endl
    //      << shift() << "ret " << (amountOfArguments * 4) << std::endl;
    code << functionName << " endp" << std::endl;
    code << std::endl;
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


bool CodeGenerator::errorWasFound() const {
    return m_ErrorFound;
}

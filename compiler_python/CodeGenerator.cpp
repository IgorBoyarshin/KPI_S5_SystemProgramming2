#include "CodeGenerator.h"


CodeGenerator::CodeGenerator(
        const std::vector<Node*> nodes,
        const std::map<std::string, Type> variablesTable) :
    m_Nodes(nodes), m_VariablesTable(variablesTable) {}


std::string CodeGenerator::generate() const {
    std::stringstream code;
    code << ".386\n"
         << ".model flat\n"
         << ".stack 4096\n"
         << "include masm32\\include\\kernel.inc\n"
         << "includelib masm32\\lib\\kernel.lib\n";

    code << ".data\n";
    for (const auto& variable : m_VariablesTable) {
        const Type variableType = variable.second;
        if (!(variableType == Type_Bool || variableType == Type_Number)) {
            continue;
        }

        const std::string variableName = variable.first;
        code << "\t"
             << variableName
             << " "
             << (variableType == Type_Bool ? "db" : "dd")
             << " 0"
             << "\n";
    }
    code << "\n";

    code << ".code\n";
    std::vector<Node*> intoMain;
    bool inFunction = false;
    for (Node* node : m_Nodes) {
        switch (node->type) {
            case FunctionNew: {
                inFunction = true;

                const NodeFunctionNew* const nodeFunctionNew =
                    static_cast<const NodeFunctionNew* const>(node);
                code << nodeFunctionNew->name
                     << " proc\n"
                     << "\tpush ebp\n"
                     << "\tmov ebp, esp\n"
                     << "\n";
                for (unsigned int i = 0; i < nodeFunctionNew->arguments.size(); i++) {
                    const std::string name = nodeFunctionNew->arguments.at(i).name;
                    const Argument arg = nodeFunctionNew->arguments.at(i);
                    const unsigned int stackIndex =
                        8 + (nodeFunctionNew->arguments.size() - i - 1) * 4;
                    code << "\tmov eax, dword prt[ebp + "
                         << stackIndex
                         << "]\n";
                    code << "\tmov "
                         << name
                         << ", eax\n";
                }
                code << "\n";
                break;
            }
            case FunctionEnd: {
                inFunction = false;

                const NodeFunctionEnd* const nodeFunctionEnd =
                    static_cast<const NodeFunctionEnd* const>(node);
                code << "\n"
                     << nodeFunctionEnd->name << "End:\n"
                     << "\tmov esp, ebp\n"
                     << "\tpop ebp\n"
                     << "\tret "
                     << (4 * nodeFunctionEnd->amountOfArguments)
                     << "\n"
                     << nodeFunctionEnd->name
                     << " endp\n";
                break;
            }
            case Statement:
                if (inFunction) {
                    const NodeStatement* const nodeStatement =
                        static_cast<const NodeStatement* const>(node);
                    code << nodeStatement->code;
                } else {
                    intoMain.push_back(node);
                }
        }
    }
    code << "\n";

    code << "\n"
         << "MAIN:\n"
         << "main proc:\n"
         << "\tpush ebp\n"
         << "\tmov ebp, esp\n"
         << "\n";
    for (const Node* node : intoMain) {
        const NodeStatement* const nodeStatement =
            static_cast<const NodeStatement* const>(node);
        code << nodeStatement->code;
    }
    code << "\tmov esp, ebp\n"
         << "\tpop ebp\n"
         << "\tret\n"
         << "main endp\n";

    return code.str();
}

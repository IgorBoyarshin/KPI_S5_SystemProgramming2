#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H


#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "Node.h"
#include "NodeFunctionNew.h"
#include "NodeFunctionEnd.h"
#include "NodeStatement.h"
#include "Type.h"


class CodeGenerator {
    public:
        CodeGenerator(
                const std::vector<Node*> nodes,
                const std::map<std::string, Type> variablesTable);

        std::string generate() const;

    private:
        const std::vector<Node*> m_Nodes;
        const std::map<std::string, Type> m_VariablesTable;
};


#endif

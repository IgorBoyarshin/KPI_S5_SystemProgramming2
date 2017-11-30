#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

#include "NodeType.h"
#include "Node.h"
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>

class CodeGenerator {
    public:
        CodeGenerator(const Node* rootNode);

        std::string generate();

        bool errorWasFound() const;

    private:
        const Node* m_RootNode;

        std::vector< std::pair<std::string, NodeType> > m_Variables;
        std::map< std::string, std::vector< std::pair<std::string, NodeType> > > m_Functions;

        mutable bool m_ErrorFound;
        mutable unsigned int m_NextAvailablyLoopIndex;

    private:
        void addVariableDeclaration(const Node* node);
        void addFunctionDeclaration(const Node* node);

        std::vector< std::pair<std::string, NodeType> >
            getAllVariableDeclarations(const Node* parentNode) const;
        std::map< std::string, std::vector< std::pair<std::string, NodeType> > >
            getAllFunctionDeclarations(const Node* parentNode) const;

        void generateDeclarationCode(std::stringstream& code, const Node* node) const;
        void generateVariableDeclarationsCode(std::stringstream& code) const;
        void generateBlockCode(std::stringstream& code, const Node* blockNode) const;
        void generateStatementListCode(
            std::stringstream& code,
            const Node* statementListNode) const;
        void generateStatementCode(
            std::stringstream& code,
            const Node* statementNode) const;
        void generateExpressionEvaluation(
            std::stringstream& code,
            const Node* expressionRoot) const;
        // Returns the amount of expressions pushed
        std::vector<NodeType> generateExpressionListPush(
            std::stringstream& code,
            const Node* expressionListRoot) const;
        void generateFunctionCallCode(
            std::stringstream& code,
            const Node* functionNode) const;

        // Will safely return if it is not a function declaration
        void generateFunctionDeclarationCode(
            std::stringstream& code,
            const Node* functionDeclarationNode) const;

        std::string getAssemblyTypeFor(NodeType nodeType) const;
        std::string getAssemblyZeroFor(NodeType nodeType) const;

        std::string shift() const;
};

#endif

#ifndef SYNTAXANALYZER_H
#define SYNTAXANALYZER_H

#include <vector>
#include <map>
#include <string>
#include <sstream> // for split
#include "Node.h"
#include "Token.h"
#include "Type.h"
#include "Piece.h"
#include "NodeFunctionNew.h"
#include "NodeFunctionEnd.h"
#include "NodeStatement.h"


class SyntaxAnalyzer {
    public:
        SyntaxAnalyzer();

        bool parse(const std::vector<Token>& tokens);

    private:
        std::vector<Node> m_Nodes;
        std::map<std::string, Type> m_VariablesTable;

        mutable unsigned int m_NextLabelNumberToUse;

    private:
        // Call me when you have:
        // - ID_VAR = .... in which case give me the index of the first token of ....
        // - ID_FUNC .... in which case give me the index of ID_FUNC(so, zero)
        // - return .... in which case give me the index of the first token of ....
        // If successsfull, returns one Piece containing all the code.
        // Otherwise returns null.
        Piece reduceExpression(const std::vector<Token> line, unsigned int startIndex, unsigned int lineIndex) const;

        // -1 === not found
        int findSimpleParentheses(const std::vector<Piece>& line) const;

        // -1 === not found
        int findCommaExprFuncEnd(const std::vector<Piece>& line) const;

        // -1 === not found
        int findExprFuncEnd(const std::vector<Piece>& line) const;

        // -1 === not found
        int findFuncBeginFuncEnd(const std::vector<Piece>& line) const;

        // returns the index in line where there is a valid operator application,
        // -1 === not found
        // -2 === error found
        int findOperator(const std::vector<Piece>& line, unsigned int lineIndex) const;
        std::vector<Piece> convertTokensIntoPieces(const std::vector<Token>& tokens, unsigned int startIndex, unsigned int lineIndex) const;

        std::string generateLogicalOperatorCode(
                const std::string& operator1Name,
                const std::string& operator2Name) const;


        std::vector<std::vector<Token>> split(const std::vector<Token>& tokens, TokenType separator) const;
        unsigned int getShiftLevel(const std::vector<Token>& line) const;
        std::vector<Token> stripFromTabs(const std::vector<Token>& line) const;
        bool areParenthesesCorrect(const std::vector<Token>& line) const;

        std::map<std::string, Type> getVariablesTable() const;
        std::vector<Node> getNodes() const;
};


#endif

#include "NodeType.h"


NodeType getNodeTypeFromString(const std::string& str) {
    if (str.compare("ID_UNDEFINED") == 0) {
        return NodeType_IdUndefined;
    } else if (str.compare("ID") == 0) {
        return NodeType_Id;
    } else if (str.compare("ID_VOID") == 0) {
        return NodeType_IdVoid;
    } else if (str.compare("ID_BOOL") == 0) {
        return NodeType_IdBool;
    } else if (str.compare("ID_INT") == 0) {
        return NodeType_IdInt;
    } else if (str.compare("ID_FLOAT") == 0) {
        return NodeType_IdFloat;
    } else if (str.compare("LITERAL_BOOL") == 0) {
        return NodeType_LiteralBool;
    } else if (str.compare("LITERAL_INT") == 0) {
        return NodeType_LiteralInt;
    } else if (str.compare("LITERAL_FLOAT") == 0) {
        return NodeType_LiteralFloat;
    } else if (str.compare("if") == 0) {
        return NodeType_KeywordIf;
    } else if (str.compare("else") == 0) {
        return NodeType_KeywordElse;
    } else if (str.compare("void") == 0) {
        return NodeType_KeywordVoid;
    } else if (str.compare("bool") == 0) {
        return NodeType_KeywordBool;
    } else if (str.compare("int") == 0) {
        return NodeType_KeywordInt;
    } else if (str.compare("float") == 0) {
        return NodeType_KeywordFloat;
    } else if (str.compare("[") == 0) {
        return NodeType_BracketsOpen;
    } else if (str.compare("]") == 0) {
        return NodeType_BracketsClose;
    } else if (str.compare("(") == 0) {
        return NodeType_ParenthesesOpen;
    } else if (str.compare(")") == 0) {
        return NodeType_ParenthesesClose;
    } else if (str.compare("{") == 0) {
        return NodeType_BracesOpen;
    } else if (str.compare("}") == 0) {
        return NodeType_BracesClose;
    } else if (str.compare("+") == 0) {
        return NodeType_OperatorPlus;
    } else if (str.compare("-") == 0) {
        return NodeType_OperatorMinus;
    } else if (str.compare("*") == 0) {
        return NodeType_OperatorMultiply;
    } else if (str.compare("/") == 0) {
        return NodeType_OperatorDivide;
    } else if (str.compare("==") == 0) {
        return NodeType_OperatorEquals;
    } else if (str.compare("!=") == 0) {
        return NodeType_OperatorNotEquals;
    } else if (str.compare("<") == 0) {
        return NodeType_OperatorLess;
    } else if (str.compare(">") == 0) {
        return NodeType_OperatorGreater;
    } else if (str.compare("<=") == 0) {
        return NodeType_OperatorLessOrEquals;
    } else if (str.compare(">=") == 0) {
        return NodeType_OperatorGreaterOrEquals;
    } else if (str.compare("=") == 0) {
        return NodeType_OperatorAssign;
    } else if (str.compare(",") == 0) {
        return NodeType_Comma;
    } else if (str.compare(";") == 0) {
        return NodeType_Semicolon;
    } else if (str.compare("BLOCK") == 0) {
        return NodeType_Block;
    } else if (str.compare("TYPED_ID_LIST") == 0) {
        return NodeType_TypedIdList;
    } else if (str.compare("TYPED_ID") == 0) {
        return NodeType_TypedId;
    } else if (str.compare("DECLARATION_LIST") == 0) {
        return NodeType_DeclarationList;
    } else if (str.compare("DECLARATION") == 0) {
        return NodeType_Declaration;
    } else if (str.compare("STATEMENT_LIST") == 0) {
        return NodeType_StatementList;
    } else if (str.compare("STATEMENT") == 0) {
        return NodeType_Statement;
    } else if (str.compare("EXPRESSION_LIST") == 0) {
        return NodeType_ExpressionList;
    } else if (str.compare("EXPRESSION_BOOL") == 0) {
        return NodeType_ExpressionBool;
    } else if (str.compare("EXPRESSION_INT") == 0) {
        return NodeType_ExpressionInt;
    } else if (str.compare("EXPRESSION_FLOAT") == 0) {
        return NodeType_ExpressionFloat;
    } else if (str.compare("FUNCTION_CALL_VOID") == 0) {
        return NodeType_FunctionCallVoid;
    } else if (str.compare("FUNCTION_CALL_BOOL") == 0) {
        return NodeType_FunctionCallBool;
    } else if (str.compare("FUNCTION_CALL_INT") == 0) {
        return NodeType_FunctionCallInt;
    } else if (str.compare("FUNCTION_CALL_FLOAT") == 0) {
        return NodeType_FunctionCallFloat;
    }

    std::cout << "[Parse Error]: In getNodeTypeFromString: unknown NodeType: "
              << str << "." << std::endl;
    return NodeType_Semicolon;
}


std::string toString(NodeType nodeType) {
    switch(nodeType) {
        case NodeType_Id:
            return "ID";
        case NodeType_IdUndefined:
            return "ID_UNDEFINED";
        case NodeType_IdVoid:
            return "ID_VOID";
        case NodeType_IdBool:
            return "ID_BOOL";
        case NodeType_IdInt:
            return "ID_INT";
        case NodeType_IdFloat:
            return "ID_FLOAT";
        case NodeType_LiteralBool:
            return "LITERAL_BOOL";
        case NodeType_LiteralInt:
            return "LITERAL_INT";
        case NodeType_LiteralFloat:
            return "LITERAl_FLOAT";
        case NodeType_KeywordIf:
            return "IF";
        case NodeType_KeywordElse:
            return "ELSE";
        case NodeType_KeywordVoid:
            return "VOID";
        case NodeType_KeywordBool:
            return "BOOL";
        case NodeType_KeywordInt:
            return "INT";
        case NodeType_KeywordFloat:
            return "FLOAT";
        case NodeType_BracketsOpen:
            return "[";
        case NodeType_BracketsClose:
            return "]";
        case NodeType_ParenthesesOpen:
            return "(";
        case NodeType_ParenthesesClose:
            return ")";
        case NodeType_BracesOpen:
            return "{";
        case NodeType_BracesClose:
            return "}";
        case NodeType_OperatorPlus:
            return "+";
        case NodeType_OperatorMinus:
            return "-";
        case NodeType_OperatorMultiply:
            return "*";
        case NodeType_OperatorDivide:
            return "/";
        case NodeType_OperatorEquals:
            return "==";
        case NodeType_OperatorNotEquals:
            return "!=";
        case NodeType_OperatorLess:
            return "<";
        case NodeType_OperatorLessOrEquals:
            return "<=";
        case NodeType_OperatorGreater:
            return ">";
        case NodeType_OperatorGreaterOrEquals:
            return ">=";
        case NodeType_OperatorAssign:
            return "=";
        case NodeType_Comma:
            return ",";
        case NodeType_Semicolon:
            return ";";

        case NodeType_Block:
            return "BLOCK";
        case NodeType_TypedIdList:
            return "TYPED_ID_LIST";
        case NodeType_TypedId:
            return "TYPED_ID";
        case NodeType_DeclarationList:
            return "DECLARATION_LIST";
        case NodeType_Declaration:
            return "DECLARATION";
        case NodeType_StatementList:
            return "STATEMENT_LIST";
        case NodeType_Statement:
            return "STATEMENT";
        case NodeType_ExpressionList:
            return "EXPRESSION_LIST";
        case NodeType_ExpressionBool:
            return "EXPRESSION_BOOL";
        case NodeType_ExpressionInt:
            return "EXPRESSION_INT";
        case NodeType_ExpressionFloat:
            return "EXPRESSION_FLOAT";
        case NodeType_FunctionCallVoid:
            return "FUNCTION_CALL_VOID";
        case NodeType_FunctionCallBool:
            return "FUNCTION_CALL_BOOL";
        case NodeType_FunctionCallInt:
            return "FUNCTION_CALL_INT";
        case NodeType_FunctionCallFloat:
            return "FUNCTION_CALL_FLOAT";

    std::cout << "[Parse Error]: In toString: unknown NodeType." << std::endl;
    return "~UNKNOWN~";
    }
}

std::ostream& operator<<(std::ostream& os, const NodeType& nodeType) {
    os << toString(nodeType);
    return os;
}

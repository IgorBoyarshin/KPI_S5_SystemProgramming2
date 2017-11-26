#ifndef RULETYPE_H
#define RULETYPE_H

enum RuleType {
    RuleType_Statement_Function,
    RuleType_Statement_IdEqExpression,
    RuleType_Statement_Semicolon,
    RuleType_Statement_Braces,
    RuleType_Statement_BracesBlock,
    RuleType_Statement_If,
    RuleType_Statement_IfElse,

    RuleType_StatementList_StatementListStatement,
    RuleType_StatementList_Statement,

    RuleType_Expression_Id,
    RuleType_Expression_Literal,
    RuleType_Expression_Function,
    RuleType_Expression_ExpressionOpExpression,
    RuleType_Expression_ParenthesesExpression,

    RuleType_Function_IdParentheses,
    RuleType_Function_IdParenthesesBlock,

    RuleType_ExpressionList_ExpressionListExpression,
    RuleType_ExpressionList_Expression
};

#endif

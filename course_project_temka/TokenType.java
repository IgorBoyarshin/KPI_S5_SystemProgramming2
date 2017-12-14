public enum TokenType {
    Id,
    LiteralInt,
    LiteralFloat,
    LiteralBool,

    OperatorAssign,
    OperatorPlus,
    OperatorMinus,
    OperatorMultiply,
    OperatorDivide,
    OperatorEquals,
    OperatorNotEquals,
    OperatorLess,
    OperatorLessOrEquals,
    OperatorGreater,
    OperatorGreaterOrEquals,

    ParenthesesOpen,
    ParenthesesClose,
    BracketsOpen,
    BracketsClose,
    BracesOpen,
    BracesClose,

    Comma,
    Semicolon,

    KeywordInt,
    KeywordFloat,
    KeywordBool,
    KeywordIf,
    KeywordElse
}

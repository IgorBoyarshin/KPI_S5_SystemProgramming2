#include "Token.h"


Token::Token(const std::string& lexeme) : _Lexeme(lexeme) {}


const Token Token::IF("if");
const Token Token::IFCOND("");
const Token Token::IFBLOCK("");
const Token Token::ELSE("else ");
const Token Token::LEFT_BRACKET("(");
const Token Token::RIGHT_BRACKET(")");
const Token Token::ASSIGN("=");
const Token Token::MUL("*");
const Token Token::SIN("sin");
const Token Token::COLUMN(";");


std::ostream& operator<<(std::ostream& os, Token token) {
    os << token._Lexeme;

    return os;
}

#include "Node.h"
#include "Token.h"

Node::Node(const Token& token) :
    _Left(nullptr), _Right(nullptr), _Token(token) {}


Node::Node(const Token& token, const Node* left, const Node* right) :
    _Left(left), _Right(right), _Token(token) {}


std::ostream& operator<<(std::ostream& os, const Node* node) {
    if (node->_Left != nullptr) {
        os << node->_Left;
    }

    os << node->_Token;

    if (node->_Right != nullptr) {
        os << node->_Right;
    }

    return os;
}

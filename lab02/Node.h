#ifndef NODE_H
#define NODE_H

#include <iostream>
#include "Token.h"

class Node {
    public:
        Node(const Token& token);
        Node(const Token& token, const Node* left, const Node* right);

    private:
        const Node* const _Left;
        const Node* const _Right;
        const Token _Token;

    public:
        friend std::ostream& operator<<(std::ostream& os, const Node* node);

};

std::ostream& operator<<(std::ostream& os, const Node* node);

#endif

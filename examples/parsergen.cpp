#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/io.hpp>
#include <iostream>
#include <string>

namespace qi = boost::spirit::qi;

struct Node {
    std::string value;
    Node* left;
    Node* right;

    Node(std::string val, Node* l = nullptr, Node* r = nullptr)
        : value(val), left(l), right(r) {}
};

bool parse(std::string::iterator& first, std::string::iterator last, Node*& tree) {
    using namespace qi;
    auto start = rule<std::string::iterator, Node*()>("start")
        = '(' >> qi::lexeme[alpha >> parse >> parse >> ')'
        ;

    bool r = parse(first, last, tree);
    if (!r || first != last) {
        tree = nullptr;
    }
    return r;
}

int main() {
    std::string input = "(a (b c) d)";
    Node* tree;
    parse(input.begin(), input.end(), tree);

    // Function to print the tree
    void printTree(Node* node, int level = 0) {
        if (node) {
            for (int i = 0; i < level; ++i) std::cout << "  ";
            std::cout << node->value << std::endl;
            printTree(node->left, level + 1);
            printTree(node->right, level + 1);
        }
    }

    printTree(tree);
    return 0;
}
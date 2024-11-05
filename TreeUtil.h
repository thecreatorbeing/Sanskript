#ifndef TREEUTIL_H
#define TREEUTIL_H

#include"Built_In.h"
#include "ConsolePrintUtil.h"
#include "CodeType.h"

class StringNode {
public:
    StringNode* parent;
    String data;
    std::vector<StringNode*> children;
    int level;

    StringNode(const String& _data, StringNode* _parent) : data(_data), parent(_parent) {
        this->level = parent ? parent->level + 1 : 0;
    }

    static StringNode* root() {
        return new StringNode("", nullptr);
    }
};

class StringTree {
public:
    StringNode* rootNode;
    StringNode* currentNode;

    StringTree() : rootNode(nullptr), currentNode(nullptr) {}

    /*Function to insert a new node as a child of a specific parent node*/
    void insert(StringNode* parent, const String& data);

    void goToParent();

    /*Function to perform pre-order traversal and print node data*/
    static void preOrderTraversal(StringNode* node);
};

/*----------------------------------------------------------------*/

class Node {
public:
    std::map<String, StrVector> data;
    CodeType type;
    Node* parent;
    std::vector<Node*> children;
    int indentLevel;

    Node(std::map<String, StrVector> _data, Node* _parent, CodeType _type, size_t _indentLevel) :
        data(_data), parent(_parent), type(_type) {
        this->indentLevel = _indentLevel;
    }

    static Node* root() {
        std::map<String, StrVector> _data;
        return new Node(_data, nullptr, CodeType::EMPTY_STMT, 0);
    }
};

class Tree {
public:
    Node* root;
    Node* current;

    Tree() : root(Node::root()), current(this->root) {}

    // Function to insert a new node as a child of a specific parent node
    void insert(std::map<String, StrVector> _mapData, Node* parent, CodeType _type, size_t _indentationLevel) {
        if (parent == nullptr) {
            //Print::yellow("parent is null!");
            // Handle case where parent is not found (improve error handling if needed)
            return;
        }
        this->current = new Node(_mapData, parent, _type, _indentationLevel);
        parent->children.push_back(this->current);
    }
    
    void goToParent() {
        if (this->current->parent == nullptr) {
            throw std::runtime_error("parent is null!");
        }
        else {
            this->current = this->current->parent;
        }
    }

    // Function to perform pre-order traversal and print node data
    void preOrderTraversal(Node* node) {
        if (node == nullptr) {
            return;
        }

        String _levelSpace = "";

        for (int i = 0; i < node->indentLevel; i++) {
            _levelSpace += "\t";
        }

        // Print current node data
        std::cout << _levelSpace << convertMapToString(node->data) << " (level: " << node->indentLevel << ")\n";
        // Recursively traverse children (pre-order)
        for (Node* child : node->children) {
            preOrderTraversal(child);
        }
    }

    String convertMapToString(const std::map<String, StrVector>& map) {
        String result;
        for (const auto& [key, value] : map) {
            result += key + ": ";
            for (const String& val : value) {
                result += val + ", ";
            }
            // Remove trailing comma and space
            result.erase(result.size() - 2, 2);
            result += "\n";
        }
        return result;
    }
};

#endif // !TREEUTIL_H
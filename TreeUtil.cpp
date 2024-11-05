#include"TreeUtil.h"

// Function to insert a new node as a child of a specific parent node
void StringTree::insert(StringNode* parent, const String& data) {
    if (parent == nullptr) {
        // Handle case where parent is not found (improve error handling if needed)
        this->currentNode = new StringNode(data, nullptr);
        this->rootNode = this->currentNode;
        return;
    }
    this->currentNode = new StringNode(data, parent);
    parent->children.push_back(this->currentNode);
}

void StringTree::goToParent() {
    if (this->currentNode->parent == nullptr) {
        //throw std::runtime_error("parent is null!");
        Print::error("parent is null!");
    }
    else {
        this->currentNode = this->currentNode->parent;
    }
}

// Function to perform pre-order traversal and print node data
void StringTree::preOrderTraversal(StringNode* node) {
    if (node == nullptr) {
        Print::warning("StrTree.preOrderTraversal node is null!");
        return;
    }

    String _levelSpace = "";

    for (int i = 0; i < node->level - 1; i++) {
        _levelSpace += "   ";
    }

    // Print current node data
    std::cout << _levelSpace << node->data << " (level: " << node->level << ")\n";
    // Recursively traverse children (pre-order)
    for (StringNode* child : node->children) {
        preOrderTraversal(child);
    }
}
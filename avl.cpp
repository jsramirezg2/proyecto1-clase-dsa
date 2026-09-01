#include "avl.h"
#include <vector>
#include <string>

struct NodeAVL {
    std::string value;
    NodeAVL* left;
    NodeAVL* right;
    int height;

    NodeAVL(const std::string& v) {
        value = v;
        left = nullptr;
        right = nullptr;
        height = 1;
    }
};

int getHeight(NodeAVL* node) {
    if (node == nullptr) {
        return 0;
    }
    return node->height;
}

int getBalance(NodeAVL* node) {
    if (node == nullptr) {
        return 0;
    }
    return getHeight(node->left) - getHeight(node->right);
}

NodeAVL* rotateRight(NodeAVL* y) {
    NodeAVL* x = y->left;
    NodeAVL* t2 = x->right;

    x->right = y;
    y->left = t2;

    y->height = 1 + getHeight(y->left);
    if (getHeight(y->right) > y->height - 1) {
        y->height = getHeight(y->right) + 1;
    }

    x->height = 1 + getHeight(x->left);
    if (getHeight(x->right) > x->height - 1) {
        x->height = getHeight(x->right) + 1;
    }

    return x;
}

NodeAVL* rotateLeft(NodeAVL* x) {
    NodeAVL* y = x->right;
    NodeAVL* t2 = y->left;

    y->left = x;
    x->right = t2;

    x->height = 1 + getHeight(x->left);
    if (getHeight(x->right) > x->height - 1) {
        x->height = getHeight(x->right) + 1;
    }

    y->height = 1 + getHeight(y->left);
    if (getHeight(y->right) > y->height - 1) {
        y->height = getHeight(y->right) + 1;
    }

    return y;
}

NodeAVL* insertNode(NodeAVL* node, const std::string& value) {
    if (node == nullptr) {
        return new NodeAVL(value);
    }

    if (value < node->value) {
        node->left = insertNode(node->left, value);
    } else if (value > node->value) {
        node->right = insertNode(node->right, value);
    } else {
        return node;
    }

    node->height = 1 + getHeight(node->left);
    if (getHeight(node->right) > node->height - 1) {
        node->height = getHeight(node->right) + 1;
    }

    int balance = getBalance(node);

    if (balance > 1 && value < node->left->value) {
        return rotateRight(node);
    }

    if (balance < -1 && value > node->right->value) {
        return rotateLeft(node);
    }

    if (balance > 1 && value > node->left->value) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    if (balance < -1 && value < node->right->value) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

void inOrder(NodeAVL* node, std::vector<std::string>& result) {
    if (node == nullptr) {
        return;
    }

    inOrder(node->left, result);
    result.push_back(node->value);
    inOrder(node->right, result);
}

std::vector<std::string> avlSort(const std::vector<std::string>& arr) {
    std::vector<std::string> result;
    NodeAVL* root = nullptr;

    for (int i = 0; i < (int)arr.size(); i++) {
        root = insertNode(root, arr[i]);
    }

    inOrder(root, result);
    return result;
}
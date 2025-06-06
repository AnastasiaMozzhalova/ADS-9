// Copyright 2022 NNTU-CS
#ifndef INCLUDE_TREE_H_
#define INCLUDE_TREE_H_
#include <vector>
#include <memory>
struct Node {
    char val;
    std::vector<std::unique_ptr<Node>> children;
    explicit Node(char v) : val(v) {}
};
class PMTree {
 private:
    std::vector<std::unique_ptr<Node>> roots;
    std::vector<char> alphabet;

    void buildTree(Node* node, std::vector<char> remaining);
    bool findPermutation(Node* node, int& counter, std::vector<char>& current,
                        std::vector<char>& result, int level);

 public:
    explicit PMTree(const std::vector<char>& input);
    ~PMTree() = default;

    void traverse(Node* node, std::vector<char>& current,
                std::vector<std::vector<char>>& result);
    const std::vector<std::unique_ptr<Node>>& getRoots() const {
        return roots;
    }
    const std::vector<char>& getAlphabet() const {
        return alphabet;
    }
};
std::vector<std::vector<char>> getAllPerms(const PMTree& tree);
std::vector<char> getPerm1(const PMTree& tree, int num);
std::vector<char> getPerm2(const PMTree& tree, int num);
#endif  // INCLUDE_TREE_H_

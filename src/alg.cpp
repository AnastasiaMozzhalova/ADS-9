// Copyright 2022 NNTU-CS
// Copyright 2023 NNTU-CS
#include <iostream>
#include <fstream>
#include <locale>
#include <cstdlib>
#include <vector>
#include <memory>
#include <utility>
#include "tree.h"

PMTree::PMTree(const std::vector<char>& input) : alphabet(input) {
    for (size_t i = 0; i < alphabet.size(); ++i) {
        for (size_t j = 0; j < alphabet.size() - 1; ++j) {
            if (alphabet[j] > alphabet[j+1]) {
                std::swap(alphabet[j], alphabet[j+1]);
            }
        }
    }
    for (char c : alphabet) {
        auto root = std::make_unique<Node>(c);
        std::vector<char> remaining;
        for (char other : alphabet) {
            if (other != c) remaining.push_back(other);
        }
        buildTree(root.get(), remaining);
        roots.push_back(std::move(root));
    }
}
void PMTree::buildTree(Node* node, std::vector<char> remaining) {
    if (remaining.empty()) return;
    for (size_t i = 0; i < remaining.size(); ++i) {
        auto child = std::make_unique<Node>(remaining[i]);
        std::vector<char> newRemaining;
        for (size_t j = 0; j < remaining.size(); ++j) {
            if (j != i) newRemaining.push_back(remaining[j]);
        }
        buildTree(child.get(), newRemaining);
        node->children.push_back(std::move(child));
    }
}
void PMTree::traverse(Node* node, std::vector<char>& current,
                     std::vector<std::vector<char>>& result) {
    current.push_back(node->val);
    if (node->children.empty()) {
        result.push_back(current);
    } else {
        for (const auto& child : node->children) {
            traverse(child.get(), current, result);
        }
    }
    current.pop_back();
}
std::vector<std::vector<char>> getAllPerms(const PMTree& tree) {
    std::vector<std::vector<char>> result;
    std::vector<char> current;
    for (const auto& root : tree.getRoots()) {
        const_cast<PMTree&>(tree).traverse(root.get(), current, result);
    }
    return result;
}
std::vector<char> getPerm1(const PMTree& tree, int num) {
    if (num <= 0) return {};
    std::vector<std::vector<char>> allPerms;
    std::vector<char> current;
    for (const auto& root : tree.getRoots()) {
        const_cast<PMTree&>(tree).traverse(root.get(), current, allPerms);
        if (allPerms.size() >= static_cast<size_t>(num)) {
            return allPerms[num-1];
        }
        num -= allPerms.size();
        allPerms.clear();
    }
    return {};
}
std::vector<char> getPerm2(const PMTree& tree, int num) {
    if (num <= 0) return {};
    const auto& alphabet = tree.getAlphabet();
    std::vector<char> result;
    std::vector<char> remaining = alphabet;
    int n = alphabet.size();
    int total = 1;
    for (int i = 2; i <= n; ++i) total *= i;
    if (num > total) return {};
    int currentNum = num - 1;
    for (int i = 0; i < n; ++i) {
        int groupSize = 1;
        for (int j = 2; j <= n - i - 1; ++j) groupSize *= j;

        int index = currentNum / groupSize;
        result.push_back(remaining[index]);
        remaining.erase(remaining.begin() + index);
        currentNum %= groupSize;
    }
    return result;
}

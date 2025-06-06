// Copyright 2022 NNTU-CS
#include  <iostream>
#include  <fstream>
#include  <locale>
#include  <cstdlib>
#include <algorithm>
#include  "tree.h"

PMTree::PMTree(const std::vector<char>& elements) : initial_elements(elements) {
  if (!elements.empty()) {
    root = std::make_shared<Node>(elements[0]);
    std::vector<char> remaining(elements.begin() + 1, elements.end());
    buildTree(root, remaining);
  }
}

void PMTree::buildTree(std::shared_ptr<Node>& node, std::vector<char>& remaining) {
  if (remaining.empty()) {
    return;
  }

  for (size_t i = 0; i < remaining.size(); ++i) {
    auto child = std::make_shared<Node>(remaining[i]);
    node->children.push_back(child);

    std::vector<char> next_remaining = remaining;
    next_remaining.erase(next_remaining.begin() + i);
    buildTree(child, next_remaining);
  }
}

void PMTree::collectPerms(std::shared_ptr<Node> node,
                  std::vector<char>& current,
                         std::vector<std::vector<char>>& result) {
  if (!node) return;

  current.push_back(node->value);

  if (node->children.empty()) {
    result.push_back(current);
  } else {
    for (auto& child : node->children) {
      collectPerms(child, current, result);
    }
  }

  current.pop_back();
}

std::vector<std::vector<char>> getAllPerms(const PMTree& tree) {
  std::vector<std::vector<char>> result;
  std::vector<char> current;
    
  auto root = tree.getRoot();
  if (!root) return result;

  PMTree* non_const_tree = const_cast<PMTree*>(&tree);
  non_const_tree->collectPerms(root, current, result);
    
  return result;
}

std::vector<char> getPerm1(const PMTree& tree, int n) {
  if (n <= 0) return std::vector<char>();
    
  auto perms = getAllPerms(tree);
  if (static_cast<size_t>(n) > perms.size()) {
    return std::vector<char>();
  }
    
  return perms[n - 1];
}

std::vector<char> PMTree::navigateToNthPerm(int n) {
  if (n <= 0) return std::vector<char>();
    
  std::vector<char> result;
  std::shared_ptr<Node> current = root;
  int remaining = n - 1;  // Convert to 0-based indexing
    
  while (current) {
    result.push_back(current->value);
        
    if (current->children.empty()) {
      if (remaining == 0) return result;
      return std::vector<char>();
    }
        
    int factorial = 1;
    for (int i = 1; i < static_cast<int>(current->children.size()); ++i) {
      factorial *= i;
    }
        
    int childIndex = remaining / factorial;
    if (childIndex >= static_cast<int>(current->children.size())) {
      return std::vector<char>();
    }
        
    remaining %= factorial;
    current = current->children[childIndex];
  }
    
  return result;
}

std::vector<char> getPerm2(const PMTree& tree, int n) {
  PMTree* non_const_tree = const_cast<PMTree*>(&tree);
  return non_const_tree->navigateToNthPerm(n);
} 

//
// Created by jewoo on 2021-11-22.
//
// 이진 탐색 트리

#include <iostream>
#include <memory>
#include <limits>
#include <numeric>
#include <queue>
#include <vector>

namespace ch14 {
    template<class T>
    struct BSTNode {
        T data;
        std::unique_ptr<BSTNode<T>> left, right;
    };

    // 이진 탁색 트리 재귀
    BSTNode<int> *SearchBST(const std::unique_ptr<BSTNode<int>> &tree,
                            int key) {
        if (tree == nullptr)
            return nullptr;
        return tree->data == key ? tree.get() : key < tree->data ? SearchBST(tree->left, key) :
                                                SearchBST(tree->right, key);
    }

    // 1. 이진 트리가 이진 탐색 트리의 속성을 만족하는지 확인하기
    bool AreKeysInRange(const std::unique_ptr<BSTNode<int>> &tree,
                        int low_range, int high_range);

    bool IsBinaryTreeBST(const std::unique_ptr<BSTNode<int>> &tree) {
        return AreKeysInRange(tree,
                              std::numeric_limits<int>::min(),
                              std::numeric_limits<int>::max());
    }

    bool AreKeysInRange(const std::unique_ptr<BSTNode<int>> &tree, int low_range, int high_range) {
        if (tree == nullptr)
            return true;
        else if (tree->data < low_range || tree->data > high_range)
            return false;
        return AreKeysInRange(tree->left, low_range, tree->data) &&
               AreKeysInRange(tree->right, tree->data, high_range);

    }

    struct QueueEntry {
        const std::unique_ptr<BSTNode<int>> &tree_node;
        int lower_bound;
        int upper_bound;
    };

    bool IsBinaryTreeBST2(const std::unique_ptr<BSTNode<int>> &tree) {
        std::queue<QueueEntry> bfs_queue;
        bfs_queue.emplace(QueueEntry{tree, std::numeric_limits<int>::min(),
                                     std::numeric_limits<int>::max()});
        while (!std::empty(bfs_queue)) {
            if (bfs_queue.front().tree_node.get()) {
                if (bfs_queue.front().tree_node->data < bfs_queue.front().lower_bound ||
                    bfs_queue.front().tree_node->data > bfs_queue.front().upper_bound) {
                    return false;
                }
                bfs_queue.emplace(QueueEntry{bfs_queue.front().tree_node->left,
                                             bfs_queue.front().lower_bound,
                                             bfs_queue.front().tree_node->data});
                bfs_queue.emplace(QueueEntry{bfs_queue.front().tree_node->right,
                                             bfs_queue.front().tree_node->data,
                                             bfs_queue.front().upper_bound});
            }
            bfs_queue.pop();
        }
        return true;
    }

    // 2. 이진 탐색 트리에서 주어진 값보다 큰 첫 번째 키 찾기
    BSTNode<int> *FindFirstGreaterThanL(const std::unique_ptr<BSTNode<int>> &tree,
                                        int k) {
        BSTNode<int> *subtree = tree.get(), *first_so_far = nullptr;
        while (subtree) {
            if (subtree->data > k) {
                first_so_far = subtree;
                subtree = subtree->left.get();
            } else { // 루프와 왼쪽 부분 트리의 모든 키는 k보다 작거나 같으므로 이들은 건너뛴다.
                subtree = subtree->right.get();
            }
        }
        return first_so_far;
    }

    // 3. 이진 탐색 트리에서 가장 큰 k개의 원소 찾기
    std::vector<

}
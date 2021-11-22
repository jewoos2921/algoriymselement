//
// Created by jewoo on 2021-11-21.
//
// 이진 트리
#include <iostream>
#include <memory>
#include <stack>

namespace ch9 {

    template<typename T>
    struct BinaryTreeNode {
        T data;
        std::unique_ptr<BinaryTreeNode<T>> left, right;
        BinaryTreeNode<T> *parent;
    };

    void TreeTraversal(const std::unique_ptr<BinaryTreeNode<int>> &root) {
        if (root) {
            //
            std::cout << " PreOrder: " << root->data << std::endl;
            TreeTraversal(root->left);
            std::cout << " InOrder: " << root->data << std::endl;
            TreeTraversal(root->right);
            std::cout << " PostOrder: " << root->data << std::endl;
        }
    }

    // 1. 이진 트리의 높이가 균형 잡혀 있는지 확인하기
    struct BalancedStatusWithHeight {
        bool balanced;
        int height;
    };

    BalancedStatusWithHeight CheckBalanced(const std::unique_ptr<BinaryTreeNode<int>> &tree);

    bool IsBalanced(const std::unique_ptr<BinaryTreeNode<int>> &tree) {
        return CheckBalanced(tree).balanced;
    }

    BalancedStatusWithHeight CheckBalanced(const std::unique_ptr<BinaryTreeNode<int>> &tree) {
        if (tree == nullptr)
            return {true, -1};

        auto left_result = CheckBalanced(tree->left);
        if (!left_result.balanced) {
            return left_result;
        }
        auto right_result = CheckBalanced(tree->right);
        if (!right_result.balanced) {
            return right_result;
        }

        bool is_balanced = abs(left_result.height - right_result.height) <= 1;
        int height = std::max(left_result.height, right_result.height) + 1;
        return {is_balanced, height};
    }

    // 2. 이진 트리가 대칭인지 확인하기

    bool CheckSymmetric(const std::unique_ptr<BinaryTreeNode<int>> &subtree_0,
                        const std::unique_ptr<BinaryTreeNode<int>> &subtree_1);

    bool IsSymmetric(std::unique_ptr<BinaryTreeNode<int>> &tree) {
        return tree == nullptr || CheckSymmetric(tree->left, tree->right);
    }

    bool CheckSymmetric(const std::unique_ptr<BinaryTreeNode<int>> &subtree_0,
                        const std::unique_ptr<BinaryTreeNode<int>> &subtree_1) {
        if (subtree_0 == nullptr && subtree_1 == nullptr) {
            return true;
        } else if (subtree_0 != nullptr && subtree_1 != nullptr) {
            return subtree_0->data == subtree_1->data &&
                   CheckSymmetric(subtree_0->left, subtree_1->right)
                   && CheckSymmetric(subtree_0->right, subtree_1->left);
        }
        return false;
    }

    struct Status {
        int num_target_nodes;

        BinaryTreeNode<int> *ancestor;
    };

    Status LcaHelper(const std::unique_ptr<BinaryTreeNode<int>> &tree,
                     const std::unique_ptr<BinaryTreeNode<int>> &node0,
                     const std::unique_ptr<BinaryTreeNode<int>> &node1);

    BinaryTreeNode<int> *Lca(const std::unique_ptr<BinaryTreeNode<int>> &tree,
                             const std::unique_ptr<BinaryTreeNode<int>> &node0,
                             const std::unique_ptr<BinaryTreeNode<int>> &node1) {
        return LcaHelper(tree, node0, node1).ancestor;
    }

    Status
    LcaHelper(const std::unique_ptr<BinaryTreeNode<int>> &tree, const std::unique_ptr<BinaryTreeNode<int>> &node0,
              const std::unique_ptr<BinaryTreeNode<int>> &node1) {

    }

    int GetDepth(const BinaryTreeNode<int> *node);

    BinaryTreeNode<int> *Lca(
            const std::unique_ptr<BinaryTreeNode<int>> &node0,
            const std::unique_ptr<BinaryTreeNode<int>> &node1) {
        BinaryTreeNode<int> *iter0 = node0.get(), *iter1 = node1.get();

        int depth0 = GetDepth(iter0), depth1 = GetDepth(iter1);

        if (depth1 > depth0) {
            std::swap(iter0, iter1);
        }
        //
        int depth_diff = abs(depth0 - depth1);
        while (depth_diff--) {
            iter0 = iter0->parent;
        }

        while (iter0 != iter1) {
            iter0 = iter0->parent, iter1 = iter1->parent;
        }
        return iter0;
    }

    int GetDepth(const BinaryTreeNode<int> *node) {
        int depth = 0;
        while (node->parent) {
            ++depth, node = node->parent;
        }
        return depth;
    }

    // 5. 이진 트리에서 루트-단말 노드 경로의 합 구하기
    int SumRootToLeafHelper(const std::unique_ptr<BinaryTreeNode<int>> &tree,
                            int partial_path_sum);

    int SumRootToLeaf(const std::unique_ptr<BinaryTreeNode<int>> &tree) {
        return SumRootToLeafHelper(tree, 0);
    }

    int SumRootToLeafHelper(const std::unique_ptr<BinaryTreeNode<int>> &tree, int partial_path_sum) {
        if (tree == nullptr)
            return 0;
        partial_path_sum = partial_path_sum * 2 + tree->data;
        if (tree->left == nullptr && tree->right == nullptr)
            return partial_path_sum;

        return SumRootToLeafHelper(tree->left, partial_path_sum) + SumRootToLeafHelper(tree->right, partial_path_sum);
    }

    // 6. 주어진 합에 해당하는 루트-단말 노드 경로 구하기
    bool HasPathSum(const std::unique_ptr<BinaryTreeNode<int>> &tree,
                    int remaining_weight) {
        if (tree == nullptr)
            return 0;
        else if (tree->left == nullptr && tree->right == nullptr)
            return remaining_weight == tree->data;

        return HasPathSum(tree->left, remaining_weight - tree->data) ||
               HasPathSum(tree->right, remaining_weight - tree->data);
    }

    // 7. 재귀를 사용하지 않고 중위 순회 구현하기

}

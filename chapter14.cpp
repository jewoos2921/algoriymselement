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
#include <map>
#include <set>
#include <functional>
#include <unordered_map>
#include <unordered_set>


namespace ch14 {
    template<class T>
    struct BSTNode {
        T data;
        std::unique_ptr<BSTNode<T>> left, right;


        // 이진 탁색 트리 재귀
        BSTNode<int> *SearchBST(const std::unique_ptr<BSTNode<int>> &tree,
                                int key) {
            if (tree == nullptr)
                return nullptr;
            return tree->data == key ? tree.get() : key < tree->data ? SearchBST(tree->left, key) :
                                                    SearchBST(tree->right, key);
        }

        // 1. 이진 트리가 이진 탐색 트리의 속성을 만족하는지 확인하기
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
        std::vector<int> FindKLargestInBST(const std::unique_ptr<BSTNode<int>> &tree, int k) {
            std::vector<int> k_largest_elements;
            FindKLargestInBSTHelper(tree, k, &k_largest_elements);
            return k_largest_elements;
        }

        void
        FindKLargestInBSTHelper(const std::unique_ptr<BSTNode<int>> &tree, int k,
                                std::vector<int> *k_largest_elements) {
            // 중위 순회를 거꾸로 수행한다.
            if (tree && std::size(*k_largest_elements) < k) {
                FindKLargestInBSTHelper(tree->right, k, k_largest_elements);
                if (std::size(*k_largest_elements) < k) {

                    k_largest_elements->emplace_back(tree->data);
                    FindKLargestInBSTHelper(tree->left, k, k_largest_elements);
                }
            }
        }

        // 4. 이진 탐색 트리에서 최소 공통 조상 구하기
        BSTNode<int> *FindLCA(const std::unique_ptr<BSTNode<int>> &tree,
                              const std::unique_ptr<BSTNode<int>> &s,
                              const std::unique_ptr<BSTNode<int>> &b) {

            auto *p = tree.get();

            while (p->data < s->data || p->data > b->data) {
                //
                while (p->data < s->data)
                    p = p->right.get();

                while (p->data > s->data)
                    p = p->left.get();
            }
            return p;
        }

        // 5. 순회한 데이터를 통해 이진 탐색 트리 재구성하기

        std::unique_ptr<BSTNode<int>> RebuildBSTFromPreorder(const std::vector<int> &preorder_sequence) {
            return RebuildBSTFromPreorderHelper(preorder_sequence, 0, std::size(preorder_sequence));
        }

        std::unique_ptr<BSTNode<int>>
        RebuildBSTFromPreorderHelper(const std::vector<int> &preorder_sequence, int start, int end) {
            if (start >= end)
                return nullptr;
            int transition_point = std::distance(
                    std::cbegin(preorder_sequence),
                    std::find_if_not(std::cbegin(preorder_sequence) + start,
                                     std::cend(preorder_sequence),
                                     [&](int a) { return a <= preorder_sequence[start]; }));
            return std::make_unique<BSTNode<int>>(
                    BSTNode<int>{
                            preorder_sequence[start],
                            RebuildBSTFromPreorderHelper(preorder_sequence, start + 1, transition_point),
                            RebuildBSTFromPreorderHelper(preorder_sequence, transition_point, end)});

        }

        std::unique_ptr<BSTNode<int>> RebuildBSTFromPreorder2(const std::vector<int> &preorder_sequence) {
            return RebuildBSTFromPreorderOnValueRange(
                    preorder_sequence, std::numeric_limits<int>::min(),
                    std::numeric_limits<int>::max(), std::make_unique<int>(0).get());
        }

        std::unique_ptr<BSTNode<int>>
        RebuildBSTFromPreorderOnValueRange(const std::vector<int> &preorder_sequence, int lower_bound, int upper_bound,
                                           int *root_idx_pointer) {
            int &root_idx = *root_idx_pointer;
            if (root_idx == preorder_sequence.size()) {
                return nullptr;
            }

            int root = preorder_sequence[root_idx];
            if (root < lower_bound || root > upper_bound)
                return nullptr;


            ++root_idx;

            auto left_subtree = RebuildBSTFromPreorderOnValueRange(
                    preorder_sequence, lower_bound, root, root_idx_pointer
            );
            auto right_subtree = RebuildBSTFromPreorderOnValueRange(
                    preorder_sequence, root_idx, upper_bound, root_idx_pointer
            );
            return std::make_unique<BSTNode<int>>(
                    BSTNode<int>{root, std::move(left_subtree), std::move(right_subtree)});
        }

        // 6. 세 개의 정렬된 배열에서 가장 가까운 원소 찾기
        int FindClosestElementsInSortedArrays(
                const std::vector<std::vector<int>> &sorted_arrays
        ) {
            struct IterTail {
                std::vector<int>::const_iterator iter, tail;
            };
            std::multimap<int, IterTail> iter_and_tail;
            for (const std::vector<int> &sorted_array: sorted_arrays) {
                iter_and_tail.emplace(sorted_array.front(),
                                      IterTail{std::cbegin(sorted_array), std::cend(sorted_array)});
            }

            int min_distance_so_far = std::numeric_limits<int>::max();
            while (true) {
                int min_value = std::cbegin(iter_and_tail)->first;
                int max_value = std::crbegin(iter_and_tail)->first;
                min_distance_so_far = std::min(max_value - min_value,
                                               min_distance_so_far);
                const auto next_min = std::next(std::cbegin(iter_and_tail)->second.iter),
                        next_end = std::cbegin(iter_and_tail)->second.tail;
                //
                if (next_min == next_end)
                    return min_distance_so_far;
                iter_and_tail.emplace(*next_min, IterTail{next_min, next_end});
                iter_and_tail.erase(std::cbegin(iter_and_tail));
            }
        }

        // 7. a + b^(1/2) 꼴의 숫자 나열하기
        struct Number {
            Number(int a, int b) : a{a}, b{b}, val(a + b * sqrt(2)) {}

            int a, b;
            double val;
        };

        std::vector<double> GenerateFirstKABSqrt2(int k) {
            std::set<Number, std::function<bool(Number, Number)>> candidates({{0, 0}},
                                                                             [](const Number &a, const Number &b) {
                                                                                 return a.val < b.val;
                                                                             });
            std::vector<double> result;
            while (std::size(result) < k) {
                auto next_smallest = std::cbegin(candidates);
                result.emplace_back(next_smallest->val);

                candidates.emplace(next_smallest->a + 1, next_smallest->b);
                candidates.emplace(next_smallest->a,
                                   next_smallest->b + 1);
                candidates.erase(next_smallest);
            }
            return result;
        }

        std::vector<double> GenerateFirstKABSqrt2_2(int k) {
            std::vector<Number> cand;
            int i = 0, j = 0;
            for (int n = 1; n < k; ++n) {
                Number cand_i_plus_1(cand[i].a + 1, cand[i].b);
                Number cand_j_plus_sqrt2(cand[j].a, cand[j].b + 1);
                cand.emplace_back(
                        std::min(cand_i_plus_1, cand_j_plus_sqrt2,
                                 [](const Number &a, const Number &b) { return a.val < b.val; }));
                if (cand_i_plus_1.val == cand.back().val) {
                    ++i;
                }
                if (cand_j_plus_sqrt2.val == cand.back().val) {
                    ++j;
                }
            }
            std::vector<double> result;
            std::transform(std::begin(cand), std::end(cand),
                           std::back_inserter(result),
                           [](const Number &c) { return c.val; });
            return result;
        }


        // 8. 정렬된 배열에서 높이가 최소인 이진 탐색 트리 만들기
        std::unique_ptr<BSTNode<int>> BuildMinHeightBSTFromSortedArray(const std::vector<int> &A) {
            return BuildMinHeightBSTFromSortedSubArray(A, 0, std::size(A));
        }

        std::unique_ptr<BSTNode<int>>
        BuildMinHeightBSTFromSortedSubArray(const std::vector<int> &A, int start, int end) {
            if (start >= end)
                return nullptr;
            int mid = start + ((end - start) / 2);
            return std::make_unique<BSTNode<int>>(A[mid], BuildMinHeightBSTFromSortedSubArray(A, start, mid),
                                                  BuildMinHeightBSTFromSortedSubArray(A, mid + 1, end));
        }

        // 9. 이진 탐색 트리의 노드 세 개가 완전히 정렬되어 있는지 확인하기
        bool PairIncludeAncestorAndDescendantOfM(
                const std::unique_ptr<BSTNode<int>> &possible_anc_or_desc_0,
                const std::unique_ptr<BSTNode<int>> &possible_anc_or_desc_1,
                const std::unique_ptr<BSTNode<int>> &middle) {
            auto *search_0 = possible_anc_or_desc_0.get();
            auto *search_1 = possible_anc_or_desc_1.get();

            while (search_0 != possible_anc_or_desc_1.get() && search_0 != middle.get() &&
                   search_1 != possible_anc_or_desc_0.get() && search_1 != middle.get() &&
                   (search_0 || search_1)) {
                if (search_0) {
                    search_0 = search_0->data > middle->data ? search_0->left.get() : search_0->right.get();
                }
                if (search_1) {
                    search_1 = search_1->data > middle->data ? search_1->left.get() : search_1->right.get();
                }
            }
            // 두 노드 모두 탐색에 실패하거나, possible_anc_or_desc_0 에서 시작해서 중간 노드를 찾지 못하고
            // possible_anc_or_desc_1에 도달했거나, possible_anc_or_desc_1에서 시작해서
            // 중간 노드를 찾지 못하고 possible_anc_or_desc_0에 도달했다면, 중간 노드는
            // possible_anc_or_desc_0과 possible_anc_or_desc_1 사이에 놓여 있지 않다.
            if ((search_0 != middle.get() && search_1 != middle.get())
                || search_0 == possible_anc_or_desc_1.get() ||
                search_1 == possible_anc_or_desc_0.get()) {
                return false;
            }

            // 코드가 여기에 도달했다면, possible_anc_or_desc_0 혹은
            // possible_anc_or_desc_1 중의 하나에서 중간 노드에 도달했다든 의미이다.
            // 이제 중간 노드에서 possible_anc_or_desc_1 혹은
            // possible_anc_or_desc_0 노드에 도달할 수 있는지 확인한다.
            return SearchTarget(middle, search_0 == middle.get() ? possible_anc_or_desc_1 : possible_anc_or_desc_0);
        }

        bool SearchTarget(const std::unique_ptr<BSTNode<int>> &from,
                          const std::unique_ptr<BSTNode<int>> &target) {
            auto *iter = from.get();
            while (iter && iter != target.get())
                iter = iter->data > target->data ? iter->left.get() : iter->right.get();

            return iter == target.get();
        }

        // 10. 범위 확인 문제
        struct Interval {
            int left, right;
        };

        std::vector<int> RangeLookupInBST(const std::unique_ptr<BSTNode<int>> &tree,
                                          const Interval &interval) {
            std::vector<int> result;
            RangeLookupInBSTHelper(tree, interval, &result);
            return result;
        }

        void RangeLookupInBSTHelper(const std::unique_ptr<BSTNode<int>> &tree,
                                    const Interval &interval, std::vector<int> *result) {
            if (tree == nullptr)
                return;
            if (interval.left <= tree->data && tree->data <= interval.right) {
                //
                RangeLookupInBSTHelper(tree->left, interval, result);
                result->template emplace_back(tree->data);
                RangeLookupInBSTHelper(tree->right, interval, result);
            } else if (interval.left > tree->data) {
                RangeLookupInBSTHelper(tree->right, interval, result);
            } else {
                RangeLookupInBSTHelper(tree->left, interval, result);
            }
        }

    };

    // 11. 신용 정보 관리 서버 설계하기
    class ClientsCreditsInfo {
    public:
        void Insert(const std::string &client_id, int c) {
            Remove(client_id);
            client_to_credit_.emplace(client_id, c - offset_);
            credit_to_clients_[c - offset_].emplace(client_id);
        }

        bool Remove(const std::string &client_id) {
            if (auto credit_iter = client_to_credit_.find(client_id);
                    credit_iter != std::end(client_to_credit_)) {
                credit_to_clients_[credit_iter->second].erase(client_id);
                if (std::empty(credit_to_clients_[credit_iter->second])) {
                    credit_to_clients_.erase(credit_iter->second);
                }
                client_to_credit_.erase(credit_iter);
                return true;
            }
            return false;
        }

        int Lookup(const std::string &client_id) const {
            auto credit_iter = client_to_credit_.find(client_id);
            return credit_iter == std::cend(client_to_credit_) ? -1 : credit_iter->second + offset_;

        }

        void AddAll(int C) { offset_ += C; }

        std::string Max() const {
            auto iter = std::crbegin(credit_to_clients_);
            return iter == std::crend(credit_to_clients_) || std::empty(iter->second) ? "" : *std::cbegin(iter->second);
        }

        friend std::ostream &operator<<(std::ostream &os,
                                        const ClientsCreditsInfo &info) {
            PrintTo(os, info.credit_to_clients_);
            return os;

        }

    private:
        int offset_{0};
        std::unordered_map<std::string, int> client_to_credit_;
        std::map<int, std::unordered_set<std::string> > credit_to_clients_;
    };
}

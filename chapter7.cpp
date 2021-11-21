//
// Created by jewoo on 2021-11-21.
//
// 연결리스트
#include <iostream>
#include <memory>
#include <array>

namespace chapter_7 {

    //연결리스트 부트캠프
    template<typename T>
    struct ListNode {
        T data;
        std::shared_ptr<ListNode<T>> next;


        std::shared_ptr<ListNode<int>> ReverseSubList(std::shared_ptr<ListNode<int>> L, int start,
                                                      int finish);

        std::shared_ptr<ListNode<int>> RemoveKthLast(const std::shared_ptr<ListNode<int>> &L,
                                                     int k);

        std::shared_ptr<ListNode<int>> EvenOddMErge(const std::shared_ptr<ListNode<int>> &L);
    };


    std::shared_ptr<ListNode<int> > SearchList(std::shared_ptr<ListNode<int>> L, int key) {
        while (L && L->data != key) {
            L = L->next;
        }
        return L;
    }

    void InsertAfter(const std::shared_ptr<ListNode<int>> &node,
                     const std::shared_ptr<ListNode<int>> &new_node) {
        new_node->next = node->next;
        node->next = new_node;
    }

    //
    void DeleteAfter(const std::shared_ptr<ListNode<int>> &node) {
        node->next = node->next->next;
    }

    void AppendNode(std::shared_ptr<ListNode<int>> *node, std::shared_ptr<ListNode<int>> *tail);

    std::shared_ptr<ListNode<int>> MergeTwoSortedLists(std::shared_ptr<ListNode<int>> L1,
                                                       std::shared_ptr<ListNode<int>> L2) {
        //
        std::shared_ptr<ListNode<int>> dummy_head(new ListNode<int>);
        auto tail = dummy_head;

        while (L1 && L2) {
            AppendNode(L1->data <= L2->data ? &L1 : &L2, &tail);
        }

        tail->next = L1 ? L1 : L2;
        return dummy_head->next;
    }

    void AppendNode(std::shared_ptr<ListNode<int>> *node, std::shared_ptr<ListNode<int>> *tail) {
        (*tail)->next = *node;
        *tail = *node;
        *node = (*node)->next;
    }

    // 7.2
    template<typename T>
    std::shared_ptr<ListNode<int>>
    ListNode<T>::ReverseSubList(std::shared_ptr<ListNode<int>> L, int start, int finish) {
        auto dummy_head = std::make_shared<ListNode<int>>(0, L);
        auto sublist_head = dummy_head;
        int k = 1;
        while (k++ < start) {
            sublist_head = sublist_head->next;
        }

        //
        auto sublist_iter = sublist_head->next;
        while (start++ < finish) {
            auto temp = sublist_iter->next;
            sublist_iter->next = temp->next;
            temp->next = sublist_head->next;
            sublist_head->next = temp;
        }
        return dummy_head->next;
    }


    // 7.3 사이클이 존재하는지 확인하기
    std::shared_ptr<ListNode<int>> HasCycle(const std::shared_ptr<ListNode<int>> &head) {
        std::shared_ptr<ListNode<int>> fast = head, slow = head;

        while (fast && fast->next) {
            slow = slow->next, fast = fast->next->next;
            if (slow == fast) {
                //
                int cycle_len = 0;
                do {
                    ++cycle_len;
                    fast = fast->next;
                } while (slow != fast);

                //
                auto cycle_len_advanced_iter = head;
                while (cycle_len--) {
                    cycle_len_advanced_iter = cycle_len_advanced_iter->next;
                }
                auto iter = head;
                //
                while (iter != cycle_len_advanced_iter) {
                    iter = iter->next;
                    cycle_len_advanced_iter = cycle_len_advanced_iter->next;
                }
                return iter; // iter 가 사이클의 시작 지점이다.
            }
        }
        return nullptr; // 사이클이 없다.
    }

    // 사이클의 길이를 계산하지 않고 사이클의 시작점을 찾는 프로그램
    std::shared_ptr<ListNode<int>> HasCycle2(const std::shared_ptr<ListNode<int>> &head) {
        std::shared_ptr<ListNode<int>> fast = head, slow = head;

        while (fast && fast->next && fast->next->next) {
            slow = slow->next, fast = fast->next->next;

            if (slow == fast) { // 사이클이 존재한다.
                // 사이클의 시작 지점을 찾는다.
                slow = head;
                // 동시에 두 포인터 앞으로 내보낸다.
                while (slow != fast) {
                    slow = slow->next, fast = fast->next;
                }
                return slow; // slow 가 사이클의 시작 지점이다.
            }
        }
        return nullptr; // 사이클이 없다.
    }

    int Length(std::shared_ptr<ListNode<int>> L);

    void AdvanceListByK(int k, std::shared_ptr<ListNode<int>> *L);

    // 7.4 사이클이 없는 두 리스트가 겹치는지 확인하기
    std::shared_ptr<ListNode<int>> OverlappingNoCycleLists(
            std::shared_ptr<ListNode<int>> L0,
            std::shared_ptr<ListNode<int>> L1
    ) {
        int L0_len = Length(L0), L1_len = Length(L1);

        AdvanceListByK(abs(L0_len - L1_len), L0_len > L1_len ? &L0 : &L1);

        while (L0 && L1 && L0 != L1) {
            L0 = L0->next, L1 = L1->next;
        }
        return L0; // L0 과 L1이 겹치지 않으면 nullptr 을 반환한다.
    }

    int Length(std::shared_ptr<ListNode<int>> L) {
        int length{0};
        while (L) {
            ++length, L = L->next;
        }
        return length;
    }

    void AdvanceListByK(int k, std::shared_ptr<ListNode<int>> *L) {
        while (k--) {
            *L = (*L)->next;
        }
    }

    // 7.5 사이클이 존재하는 두 리스트가 겹치는지 확인하기
    std::shared_ptr<ListNode<int>> OverlappingLists(std::shared_ptr<ListNode<int>> L0,
                                                    std::shared_ptr<ListNode<int>> L1) {

        auto root0 = HasCycle(L0), root1 = HasCycle(L1);

        if (!root0 && !root1) {
            // 두 리스트 모두 사이클이 존재 하지 않는다.
            return OverlappingNoCycleLists(L0, L1);
        } else if ((root0 && !root1) || (!root0 && root1)) {
            // 하나는 사이클이 존재하고 하나는 존재하지 않는다.
            return nullptr;
        }
        auto temp = root1;
        do {
            temp = temp->next;
        } while (temp != root0 && temp != root1);

        return temp == root0 ? root1 : nullptr;
    }

    int Distance(std::shared_ptr<ListNode<int>> a, std::shared_ptr<ListNode<int>> b) {
        int dis{0};
        while (a != b) {
            a = a->next, ++dis;
        }
        return dis;
    }

    // 7.6 단순 연결리스트에서 노드 삭제하기
    void DeletionFromList(const std::shared_ptr<ListNode<int>> &node_to_delete) {
        node_to_delete->data = node_to_delete->next->data;
        node_to_delete->next = node_to_delete->next->next;
    }

    // 7.7 리스트에서 뒤에서 k번째 원소 삭제하기
    template<typename T>
    std::shared_ptr<ListNode<int>> ListNode<T>::RemoveKthLast(const std::shared_ptr<ListNode<int>> &L, int k) {
        // L에 k개 이상의 노드가 있다고 가정하고, 뒤에서 k번째 노드를 삭제한다.
        auto dummy_head = std::make_shared<ListNode<int>>(0, L);
        auto first = dummy_head->next;
        while (k--) {
            first = first->next;
        }

        auto second = dummy_head;
        while (first) {
            second = second->next, first = first->next;
        }

        // second 가 뒤에서 k + 1 번째 노드를 가리키므로, 그 다음 노드를 삭제한다.
        second->next = second->next->next;
        return dummy_head->next;
    }

    // 7.8 정렬된 리스트에서 중복된 원소 삭제하기
    std::shared_ptr<ListNode<int>> RemoveDuplicates(const std::shared_ptr<ListNode<int>> &L) {
        auto iter = L;
        while (iter) {
            //
            auto next_distinct = iter->next;
            while (next_distinct && next_distinct->data == iter->data) {
                next_distinct = next_distinct->next;
            }
            iter->next = next_distinct;
            iter = next_distinct;
        }
        return L;
    }

    // 7.9 단순 연결리스트에서 오른쪽 원형 시프트 구현하기
    std::shared_ptr<ListNode<int>> CyclicallyRightShiftList(std::shared_ptr<ListNode<int>> L,
                                                            int k) {
        if (L == nullptr)
            return L;

        // L의 길이와 마지막 노드를 구한다.
        auto tail = L;
        int n = 1;
        while (tail->next) {
            ++n, tail = tail->next;
        }
        k %= n;
        if (k == 0)
            return L;

        tail->next = L;
        int steps_to_new_head = n - k;
        auto new_tail = tail;
        while (steps_to_new_head--) {
            new_tail = new_tail->next;
        }
        auto new_head = new_tail->next;
        new_tail->next = nullptr;
        return new_head;
    }

    // 7.10 짝수-홓수 병합 구현하기
    template<typename T>
    std::shared_ptr<ListNode<int>> ListNode<T>::EvenOddMErge(const std::shared_ptr<ListNode<int>> &L) {
        if (L == nullptr)
            return L;

        auto even_dummy_head = std::make_shared<ListNode<int>>(0, nullptr);
        auto odd_dummy_head = std::make_shared<ListNode<int>>(0, nullptr);
        std::array<std::shared_ptr<ListNode<int>>, 2> tails = {even_dummy_head, odd_dummy_head};

        int turn = 0;
        for (auto iter = L; iter; iter = iter->next) {
            tails[turn]->next = iter;
            tails[turn] = tails[turn]->next;
            turn ^= 1;
        }
        tails[1]->next = nullptr;
        tails[0]->next = odd_dummy_head->next;
        return even_dummy_head->next;

    }

    // 7.11 단순 연결리스트가 회문인지 확인하기

    std::shared_ptr<ListNode<int>> ReverseList(std::shared_ptr<ListNode<int>> L);

    bool IsLinkedListAPalindrome(std::shared_ptr<ListNode<int>> L) {

        std::shared_ptr<ListNode<int>>
                slow = L, fast = L;
        while (fast && fast->next) {
            fast = fast->next->next, slow = slow->next;
        }

        //
        auto first_half_iter = L, second_half_iter = ReverseList(slow);
        while (second_half_iter && first_half_iter) {
            if (second_half_iter->data != first_half_iter->data)
                return false;
            second_half_iter = second_half_iter->next;

        }

        return true;
    }
    std::shared_ptr<ListNode<int>> ReverseList(std::shared_ptr<ListNode<int>> L) {
        return std::shared_ptr<ListNode<int>>();
    }
}

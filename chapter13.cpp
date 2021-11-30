//
// Created by jewoo on 2021-11-29.
//
// 정렬
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <memory>

namespace ch13 {

    struct Student {
        bool operator<(const Student &that) const { return name < that.name; }

        std::string name;
        double grade_point_average;

        void SortByName(std::vector<Student> *students) {
            std::sort(std::begin(*students), std::end(*students));
        }

        void SortByGPA(std::vector<Student> *students) {
            std::sort(std::begin(*students), std::end(*students),
                      [](const Student &a, const Student &b) {
                          return a.grade_point_average >= b.grade_point_average;
                      });
        }
    };

    // 13.1 정렬된 두 배열의 교집합 구하기
    std::vector<int> IntersectTwoSortedArrays(const std::vector<int> &A,
                                              const std::vector<int> &B) {
        std::vector<int> intersection_A_B;
        for (int i = 0; i < std::size(A); ++i) {
            if ((!i || A[i] != A[i - 1]) && std::find(std::begin(B), std::end(B), A[i]) != std::end(B)) {
                intersection_A_B.emplace_back(A[i]);
            }
        }
        return intersection_A_B;
    }

    std::vector<int> IntersectTwoSortedArrays2(const std::vector<int> &A,
                                               const std::vector<int> &B) {
        std::vector<int> intersection_A_B;
        for (int i = 0; i < A.size(); ++i) {
            if ((i == 0 || A[i] != A[i - 1]) &&
                std::binary_search(B.cbegin(), B.cend(), A[i])) {
                intersection_A_B.emplace_back(A[i]);
            }
        }
        return intersection_A_B;
    }

    std::vector<int> IntersectTwoSortedArrays3(const std::vector<int> &A,
                                               const std::vector<int> &B) {
        std::vector<int> intersection_A_B;
        int i = 0;
        int j = 0;
        while (i < A.size() && j < B.size()) {
            if (A[i] == B[j] && (i == 0 || A[i] != A[i - 1])) {
                intersection_A_B.emplace_back(A[i]);
                ++i, ++j;
            } else if (A[i] < B[j]) {
                ++i;
            } else {
                ++j;
            }
        }
        return intersection_A_B;
    }

    // 13.2 정렬된 두 배열 합치기
    void MergeTwoSortedArrays(
            std::vector<int> &A,
            int m,
            const std::vector<int> &B, int n) {
        int a = m - 1, b = n - 1, write_idx = m + n - 1;
        while (a >= 0 && b >= 0) {
            A[write_idx--] = A[a] > B[b] ? A[a--] : B[b--];
        }
        while (b >= 0) {
            A[write_idx--] = B[b--];
        }
    }

    // 13.3 H-인덱스 계산하기
    int HIndex(std::vector<int> citations) {
        std::sort(std::begin(citations), std::end(citations));
        const int n = citations.size();
        for (int i = 0; i < citations.size(); ++i) {
            if (citations[i] >= n - i)
                return n - i;
        }
        return 0;
    }

    // 13.4 성을 제외한 중복되는 이름 제거하기
    struct Name {
        std::string first_name, last_name;

        bool operator==(const Name &that) const {
            return first_name == that.first_name;
        }

        bool operator<(const Name &that) const {
            return first_name != that.first_name ?
                   first_name < that.first_name :
                   last_name < that.last_name;
        }

        void EliminateDuplicate(std::vector<Name> *names) {
            std::sort(std::begin(*names),
                      std::end(*names)); // 같은 원소를 가까이에 놓는다.

            // unique 는 인접한 중복을 제거하고,
            // 제거되지 않은 마지막 원소 다음 위치에 대한 반복자를 반환한다.
            // erase 는 고유한 원소로 이름을 제한하는 역할을 한다.
            names->erase(std::unique(std::begin(*names),
                                     std::end(*names)),
                         std::end(*names));
        }
    };

    // 13.5 동전으로 만들 수 없는 가장 작은 숫자 구하기
    int SmallestNonconstructibleValue(std::vector<int> A) {
        std::sort(std::begin(A), std::end(A));

        int max_constructible_value = 0;
        for (int a: A) {
            if (a > max_constructible_value + 1)
                break;
            max_constructible_value += a;
        }
        return max_constructible_value + 1;
    }

    // 13.6 달력 만들기
    struct Event {
        int start, finish;

        int FindMaxSumultaneousEvents(const std::vector<Event> &A) {
            struct EndPoint {
                int time;
                bool is_start;
            };

            // 모든 점을 포함하는 배열을 만든다.
            std::vector<EndPoint> E;
            for (const Event &event: A) {
                E.push_back({event.start, true});
                E.push_back({event.finish, false});
            }

            // 시간순으로 끝점을 정렬한다.
            // 점의 위치가 같을 때는 시작점을 끝점보다 앞에 오도록 한다.
            std::sort(std::begin(E), std::end(E),
                      [](const EndPoint &a, const EndPoint &b) {
                          return a.time != b.time ? a.time < b.time : (a.is_start && !b.is_start);
                      });

            // 동시에 발생하는 이벤트의 개수를 추적한다.
            // 그리고 그중에서 가장 많은 이벤트의 개수를 저장한다.
            int max_num_simultaneous_evnets = 0,
                    num_sumultaneous_events = 0;

            for (const EndPoint &endPoint: E) {
                if (endPoint.is_start) {
                    ++num_sumultaneous_events;
                    max_num_simultaneous_evnets =
                            std::max(num_sumultaneous_events,
                                     max_num_simultaneous_evnets);
                } else {
                    --num_sumultaneous_events;
                }
            }
            return max_num_simultaneous_evnets;
        }
    };

    // 13.7 구간 합치기
    struct Interval {
        int left, right;

        std::vector<Interval> AddInterval(const std::vector<Interval> &disjoint_intervals,
                                          Interval new_interval) {
            size_t i{0};
            std::vector<Interval> result;

            //
            while (i < std::size(disjoint_intervals) &&
                   new_interval.left > disjoint_intervals[i].right) {
                result.emplace_back(disjoint_intervals[i++]);
            }

            //
            while (i < std::size(disjoint_intervals) &&
                   new_interval.right >= disjoint_intervals[i].left) {
                //
                //
                new_interval = {std::min(new_interval.left, disjoint_intervals[i].left),
                                std::max(new_interval.right, disjoint_intervals[i].right)};
                ++i;
            }
            // disjoint_intervals 에서 new_interval 보다 뒤에 등장하는 구간들을 처리한다.
            result.emplace_back(new_interval);
            result.insert(std::end(result), std::begin(disjoint_intervals) + i, std::end(disjoint_intervals));
            return result;
        }
    };

    // 13.8 구간의 합 구하기
    struct Intervals {
        struct EndPoint {
            bool is_closed;
            int val;
        };
        EndPoint left, right;

        std::vector<Intervals> UnionOfIntervals(std::vector<Intervals> intervals) {
            // 입력이 비어 있는 경우
            if (std::empty(intervals))
                return {};

            // 왼쪽 끝점을 기준으로 구간을 정렬한다.
            std::sort(std::begin(intervals), std::end(intervals),
                      [](const Intervals &a, const Intervals &b) {
                          if (a.left.val != b.left.val)
                              return a.left.val < b.left.val;
                          return a.left.is_closed && !b.left.is_closed;
                      });

            std::vector<Intervals> result;

            for (Intervals i: intervals) {
                if (!std::empty(result) &&
                    (i.left.val < result.back().right.val ||
                     (i.left.val == result.back().right.val &&
                      (i.left.is_closed || result.back().right.is_closed)))) {
                    if (i.right.val > result.back().right.val ||
                        (i.right.val == result.back().right.val &&
                         i.right.is_closed)) {
                        result.back().right = i.right;
                    }
                } else {
                    result.emplace_back(i);
                }
            }
            return result;
        }
    };

    // 13.9 반복되는 원소가 많은 배열, 정렬하고 나누기
    struct Person {
        int age;
        std::string name;

        void GroupByAge(std::vector<Person> *people) {
            std::unordered_map<int, int> age_to_count;
            for (const Person &p: *people) {
                ++age_to_count[p.age];
            }
            std::unordered_map<int, int> age_to_offset;
            int offset{0};
            for (const auto &[age, count]: age_to_count) {
                age_to_offset[age] = offset;
                offset += count;
            }

            while (!std::empty(age_to_offset)) {
                auto from = std::begin(age_to_offset);
                auto to = age_to_offset.find((*people)[from->second].age);
                std::swap((*people)[from->second], (*people)[to->second]);
                //
                --age_to_count[to->first];
                if (age_to_count[to->first] > 0) {
                    ++to->second;
                } else {
                    age_to_offset.erase(to);
                }
            }
        }
    };

    // 13.10 팀 사진 찍는 날 - 1
    class Team {
    public:
        explicit Team(const std::vector<int> &height) {
            std::transform(std::begin(height),
                           std::end(height),
                           std::back_inserter(players_), [](int h) { return Player{h}; });
        }

        // team0 을 team1의 앞에 줄세울 수 있는지 확인한다.
        static bool ValidPlacementExists(const Team &team0, const Team &team1) {
            std::vector<Player> team0_sorted(team0.SortPlayersByHeight());
            std::vector<Player> team1_sorted(team1.SortPlayersByHeight());
            for (int i = 0; i < std::size(team0_sorted) && i < std::size(team1_sorted); ++i) {
                if (!(team0_sorted[i] < team1_sorted[i]))
                    // team0_sorted[i]는 team1_sorted[i] 뒤에 세울 수 없다.
                    return false;
            }
            return true;
        }

    private:
        struct Player {
            bool operator<(const Player &that) const {
                return height < that.height;
            }

            int height;
        };

        std::vector<Player> SortPlayersByHeight() const {
            std::vector<Player> sorted_players(players_);
            std::sort(std::begin(sorted_players),
                      std::end(sorted_players));
            return sorted_players;
        }

        std::vector<Player> players_;
    };

    template<typename T>
    struct ListNode {
        T data;

        // 여러 노드가 단일 노드를 가리킬 수 있으므로
        // 공유 포인터를 사용해야 한다.
        // 0->1->2->3->4
        std::shared_ptr<ListNode<T>> next;

        // 키 검색하기
        std::shared_ptr<ListNode<int>> SearchList(
                std::shared_ptr<ListNode<int>> L, int key) {
            while (L && L->data != key)
                L = L->next;
            // 키가 리스트에 없으면 L은 null이 된다.
            return L;
        }

        // 특정 노드 다음에 새로운 노드를 삽입하기
        // node 다음에  new_node 삽입
        void InsertAfter(const std::shared_ptr<ListNode<int>> &node,
                         const std::shared_ptr<ListNode<int>> &new_node) {
            new_node->next = node->next;
            node->next = new_node;
        }

        // 노드 삭제하기
        // 바로 다음 노드 삭제하기, 노드는 테일이 아니라고 가정해도 된다.
        void DeleteAfter(const std::shared_ptr<ListNode<int>> &node) {
            node->next = node->next->next;
        }

        // 13.11 리스트를 빠르게 정렬하는 알고리즘 구현하기
        std::shared_ptr<ListNode<int>> InsertionSort(const std::shared_ptr<ListNode<int>> &L) {
            auto dummy_head = std::make_shared<ListNode<int>>(ListNode<int>{0, L});
            auto iter = L;
            //
            //
            //
            //
            while (iter && iter->next) {
                if (iter->data > iter->next->data) {
                    auto target = iter->next, pre = dummy_head;
                    while (pre->next->data < target->data)
                        pre = pre->next;
                    auto temp = pre->next;
                    pre->next = target;
                    iter->next = target->next;
                    target->next = temp;
                } else {
                    iter = iter->next;
                }
            }
            return dummy_head->next;
        }

        std::shared_ptr<ListNode<int>> StableSortList(std::shared_ptr<ListNode<int>> L) {

            // 기저 사례: L이 비어 있거나 노드가 하나인 경우에는 단순히 반환한다.
            if (L == nullptr || L->next == nullptr)
                return L;

            // 느린 포인터와 빠른 포인터를 사용해서 L의 중앙을 찾는다.
            std::shared_ptr<ListNode<int>> pre_show = nullptr, slow = L, fast = L;
            while (fast && fast->next) {
                pre_show = slow;
                fast = fast->next->next, slow = slow->next;
            }
            if (pre_show) {
                // 크기가 동일한 두 개의 리스트로 나눈다.
                pre_show->next = nullptr;
            }
            return MergeTwoSortedLists(StableSortList(L), StableSortList(slow));
        }

        //  7.1 두개의 정렬된 리스트 합치기
        std::shared_ptr<ListNode<int>>
        MergeTwoSortedLists(std::shared_ptr<ListNode<int>> L1, std::shared_ptr<ListNode<int>> L2) {
            //
            std::shared_ptr<ListNode<int>> dummy_head(new ListNode<int>);
            auto tail = dummy_head;

            while (L1 && L2) {
                AppendNode(L1->data <= L2->data ? &L1 : &L2, &tail);
            }

            //
            tail->next = L1 ? L1 : L2;
            return dummy_head->next;
        }

        void AppendNode(std::shared_ptr<ListNode<int>> *node, std::shared_ptr<ListNode<int>> *tail) {
            (*tail)->next = *node;
            *tail = *node; // 테일을 갱신
            *node = (*node)->next;
        }
    };

    // 13.12 급여의 한계 구하기
    double FindSalaryCap(int target_payroll, std::vector<int> current_salaries) {
        std::sort(std::begin(current_salaries),
                  std::end(current_salaries));
        double unadjusted_salary_sum{0.0};
        for (int i = 0; i < std::size(current_salaries); ++i) {
            const int adjusted_people = std::size(current_salaries) - i;
            const double adjusted_salary_sum = current_salaries[i] * adjusted_people;
            if (unadjusted_salary_sum + adjusted_salary_sum >= target_payroll)
                return (target_payroll - unadjusted_salary_sum) / adjusted_people;
            unadjusted_salary_sum += current_salaries[i];
        }
        // target_payroll > 기존의 급여이므로 해결책이 존재 안함
        return -1.0;
    }

}
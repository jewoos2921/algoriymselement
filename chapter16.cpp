//
// Created by jewoo on 2021-11-29.
//
// 동적 프로그래밍
// 동적 프로그래밍을 푸는 핵심은 부분 문제로 나누는 방법을 찾는 것이다.

// 부분 문제의 해법을 찾으면 원래 문제도 비교적 쉽게 해결할 수 있다.
// 부분 문제의 해법을 캐시에 저장할 수 있다.

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

namespace ch16 {
    std::unordered_map<int, int> cache;

    int Fibonacci(int n) {
        if (n <= 1)
            return n;
        else if (!cache.count(n))
            cache[n] = Fibonacci(n - 1) + Fibonacci(n - 2);
        return cache[n];
    }

    int Fibonacci2(int n) {
        if (n <= 1)
            return n;

        int f_minus_2 = 0, f_minus_1 = 1;
        for (int i = 2; i <= n; ++i) {
            int f = f_minus_2 + f_minus_1;
            f_minus_2 = f_minus_1;
            f_minus_1 = f;
        }
        return f_minus_1;
    }

    int FindMaximumSubArray(const std::vector<int> &A) {
        int max_seen = 0, max_end = 0;
        for (int a: A) {
            max_end = std::max(a, a + max_end);
            max_seen = std::max(max_seen, max_end);
        }
        return max_seen;
    }


    // 16.1 가능한 점수가 몇 개인지 구하기
    int NumCombinationForFinalScore(int final_score,
                                    const std::vector<int> &individual_play_scores) {
        std::vector<std::vector<int>> num_combinations_for_score(
                std::size(individual_play_scores), std::vector<int>(final_score + 1, 0)
        );
        for (int i = 0; i < std::size(individual_play_scores); ++i) {
            num_combinations_for_score[i][0] = 1;
            for (int j = 1; j <= final_score; ++j) {
                int without_this_play = i >= 1 ? num_combinations_for_score[i - 1][j] : 0;
                int with_this_play =
                        j >= individual_play_scores[i] ? num_combinations_for_score[i][j - individual_play_scores[i]]
                                                       : 0;
                num_combinations_for_score[i][j] = with_this_play + without_this_play;
            }
        }
        return num_combinations_for_score.back().back();
    }

    // 16.2 레벤슈타인 거리 구하기

    int ComputeDistanceBetweenPrefixes(
            const std::string &A, int A_idx, const std::string &B, int B_idx,
            std::vector<std::vector<int>> *distance_between_prefixes_ptr);

    int LevenshteinDistance(const std::string &A, const std::string &B) {
        return ComputeDistanceBetweenPrefixes(
                A, std::size(A) - 1,
                B, std::size(B) - 1,
                std::make_unique<std::vector<std::vector<int>>>(std::size(A),
                                                                std::vector<int>(std::size(B), 1)).get());
    }

    int ComputeDistanceBetweenPrefixes(const std::string &A, int A_idx, const std::string &B, int B_idx,
                                       std::vector<std::vector<int>> *distance_between_prefixes_ptr) {

        std::vector<std::vector<int>> &distance_between_prefixes = *distance_between_prefixes_ptr;

        if (A_idx < 0) {
            return B_idx + 1;
        } else if (B_idx < 0) {
            return A_idx + 1;
        }

        if (distance_between_prefixes[A_idx][B_idx] == -1) {
            if (A[A_idx] == B[B_idx]) {
                distance_between_prefixes[A_idx][B_idx] = ComputeDistanceBetweenPrefixes(
                        A, A_idx - 1, B, B_idx - 1, distance_between_prefixes_ptr);
            } else {
                int substitute_last = ComputeDistanceBetweenPrefixes(A, A_idx - 1, B, B_idx - 1,
                                                                     distance_between_prefixes_ptr);
                int add_last = ComputeDistanceBetweenPrefixes(A, A_idx,
                                                              B, B_idx - 1, distance_between_prefixes_ptr);
                int delete_last = ComputeDistanceBetweenPrefixes(
                        A, A_idx - 1, B, B_idx, distance_between_prefixes_ptr);
                distance_between_prefixes[A_idx][B_idx] =
                        1 + std::min({substitute_last, add_last, delete_last});
            }
        }
        return distance_between_prefixes[A_idx][B_idx];
    }

    // 16.3 2차원 배열을 순회할 수 있는 방법의 개수 구하기
    int ComputeNumberOfWaysToXY(int x, int y, std::vector<std::vector<int>> *number_of_ways_ptr);

    int NumberOfWays(int n, int m) {
        return ComputeNumberOfWaysToXY(n - 1, m - 1,
                                       std::make_unique<std::vector<std::vector<int>>>(n,
                                                                                       std::vector<int>(m, 0)).get());
    }

    int ComputeNumberOfWaysToXY(int x, int y, std::vector<std::vector<int>> *number_of_ways_ptr) {

        if (x == 0 && y == 0)
            return 1;

        std::vector<std::vector<int>> &number_of_ways = *number_of_ways_ptr;

        if (number_of_ways[x][y] == 0) {
            int ways_top =
                    x == 0 ? 0 : ComputeNumberOfWaysToXY(x - 1, y, number_of_ways_ptr);
            int ways_left =
                    y == 0 ? 0 : ComputeNumberOfWaysToXY(x, y - 1, number_of_ways_ptr);
            number_of_ways[x][y] = ways_top + ways_left;
        }
        return number_of_ways[x][y];
    }

    // 16.4 이항계수 구하기
    int ComputeXChooseY(int x, int y, std::vector<std::vector<int>> *x_choose_y_ptr);

    int ComputeBinomialCoefficient(int n, int k) {
        return ComputeXChooseY(n, k,
                               std::make_unique<std::vector<std::vector<int>>>(n + 1,
                                                                               std::vector<int>(k + 1, 0)).get());
    }

    int ComputeXChooseY(int x, int y, std::vector<std::vector<int>> *x_choose_y_ptr) {

        if (x == 0 && y == 0)
            return 1;

        std::vector<std::vector<int>> &x_choose_y = *x_choose_y_ptr;
        if (x_choose_y[x][y] == 0) {
            int without_y = ComputeXChooseY(x - 1, y, x_choose_y_ptr);
            int with_y = ComputeXChooseY(x - 1, y - 1, x_choose_y_ptr);
            x_choose_y[x][y] = without_y + with_y;
        }
        return x_choose_y[x][y];
    }

    // 16.5 2차원 배열에서 수열 찾기
    struct HashTable {
        size_t operator()(const std::tuple<int, int, int> &t) const {
            return std::hash<int>()(std::get<0>(t) ^ std::get<1>(t) * 1021 ^ std::get<2>(t) * 1048573);
        }

        bool IsPatternContainedInGrid(const std::vector<std::vector<int>> &grid,
                                      const std::vector<int> &pattern) {
            for (int i = 0; i < std::size(grid); ++i) {
                for (int j = 0; j < std::size(grid[i]); ++j) {
                    if (IsPatternSuffixContainedStartingAtXY(grid, i, j, pattern, 0,
                                                             std::make_unique<std::unordered_set<std::tuple<int, int, int>, HashTable>>().get())) {
                        return true;
                    }
                }
            }
        }

        bool IsPatternSuffixContainedStartingAtXY(
                const std::vector<std::vector<int>> &grid,
                int x, int y,
                const std::vector<int> &pattern,
                int offset,
                std::unordered_set<std::tuple<int, int, int>, HashTable> *previous_attempts) {

            if (std::size(pattern) == offset)
                return true;

            if (x < 0 || x >= std::size(grid) || y < 0 || y >= std::size(grid[x]) ||
                previous_attempts->find(std::make_tuple(x, y, offset)) != std::cend(*previous_attempts)) {
                return false;
            }

            for (const auto &[next_x, next_y]:
                    std::vector<std::pair<int, int>>{{x - 1, y},
                                                     {x + 1, y},
                                                     {x,     y - 1},
                                                     {x,     y + 1}}) {
                if (IsPatternSuffixContainedStartingAtXY(grid, next_x,
                                                         next_y, pattern,
                                                         offset + 1,
                                                         previous_attempts)) {
                    return true;
                }
            }
            previous_attempts->emplace(x, y, offset);
            return false;
        }
    };

    // 16.6 배낭 문제
    struct Item {
        int weight, value;

        int OptimumSubjectToCapacity(const std::vector<Item> &items,
                                     int capacity) {
            return OptimumSubjectToItemAndCapacity(
                    items, std::size(items) - 1, capacity,
                    std::make_unique<std::vector<std::vector<int>>>(std::size(items),
                                                                    std::vector<int>(capacity + 1, -1)).get());
        }

        int OptimumSubjectToItemAndCapacity(const std::vector<Item> &items, int k,
                                            int available_capacity,
                                            std::vector<std::vector<int>> *V_ptr) {
            if (k < 0)
                return 0;

            std::vector<std::vector<int>> &V = *V_ptr;
            if (V[k][available_capacity] == -1) {
                int without_curr_item = OptimumSubjectToItemAndCapacity(items, k - 1,
                                                                        available_capacity, V_ptr);
                int with_curr_item =
                        available_capacity < items[k].weight ? 0
                                                             : items[k].value +
                                                               OptimumSubjectToItemAndCapacity(items, k - 1,
                                                                                               available_capacity -
                                                                                               items[k].weight, V_ptr);
                V[k][available_capacity] = std::max(without_curr_item, with_curr_item);
            }
            return V[k][available_capacity];
        }
    };

    // 16.7 BED BATH AND BEYOND .COM 문제
    std::vector<std::string> DecomposeIntoDictionaryWords(
            const std::string &domain,
            const std::unordered_set<std::string> &dictionary) {
        //
        //
        //

        std::vector<int> last_length(std::size(domain), -1);

        for (int i = 0; i < std::size(domain); ++i) {

            if (dictionary.count(domain.substr(0, i + 1))) {
                last_length[i] = i + 1;
                continue;
            }
            for (int j = 0; j < i; ++j) {
                if (last_length[j] != -1 &&
                    dictionary.count(domain.substr(j + 1, i - j))) {
                    last_length[i] = i - j;
                    break;
                }
            }
        }

        std::vector<std::string> decompositions;
        if (last_length.back() != -1) {
            int idx = std::size(domain) - 1;
            while (idx >= 0) {
                decompositions.emplace_back(
                        domain.substr(idx + 1 - last_length[idx],
                                      last_length[idx]));
                idx -= last_length[idx];
            }
            std::reverse(std::begin(decompositions), std::end(decompositions));
        }
        return decompositions;
    }

    // 16.8 삼각형에서 무게가 가장 작은 경로 찾기
    int MinimumPathWeight(const std::vector<std::vector<int>> &triangle) {

        if (std::empty(triangle))
            return 0;

        std::vector<int> prev_row(triangle.front());
        for (int i = 0; i < std::size(triangle); ++i) {
            std::vector<int> curr_row(triangle[i]);
            curr_row.front() += prev_row.front();

            for (int j = 1; j < std::size(curr_row); ++j) {
                curr_row[j] += std::min(prev_row[j - 1], prev_row[j]);
            }
            curr_row.back() += prev_row.back();
        }
        return *std::min_element(std::cbegin(prev_row), std::cend(prev_row));
    }

    // 16.9 합이 최대가 되도록 동전 선택하기
    int ComputeMaximumRevenueForRange(
            const std::vector<int> &coins, int a, int b,
            std::vector<std::vector<int>> *maximum_revenue_for_range_ptr);

    int MaximumRevenue(const std::vector<int> &coins) {
        std::vector<std::vector<int> > maximum_revenue_for_range(std::size(coins),
                                                                 std::vector<int>(std::size(coins), 0));
        return ComputeMaximumRevenueForRange(coins, 0, std::size(coins) - 1,
                                             std::make_unique<std::vector<std::vector<int>>>(std::size(coins),
                                                                                             std::vector<int>(
                                                                                                     std::size(coins),
                                                                                                     0)).get());
    }

    int ComputeMaximumRevenueForRange(const std::vector<int> &coins, int a, int b,
                                      std::vector<std::vector<int>> *maximum_revenue_for_range_ptr) {
        if (a > b)
            // 남아 있는 동전이 없다.
            return 0;

        std::vector<std::vector<int>> &maximum_revenue_for_range = *maximum_revenue_for_range_ptr;

        if (maximum_revenue_for_range[a][b] == 0) {
            int max_revenue_a =
                    coins[a] + std::min(ComputeMaximumRevenueForRange(coins,
                                                                      a + 2, b, maximum_revenue_for_range_ptr),
                                        ComputeMaximumRevenueForRange(coins, a + 1, b - 1,
                                                                      maximum_revenue_for_range_ptr));
            int max_revenue_b =
                    coins[b] +
                    std::min(ComputeMaximumRevenueForRange(coins, a + 1, b - 1, maximum_revenue_for_range_ptr),
                             ComputeMaximumRevenueForRange(coins, a, b - 2, maximum_revenue_for_range_ptr));
            maximum_revenue_for_range[a][b] = std::max(max_revenue_a, max_revenue_b);

        }
        return maximum_revenue_for_range[a][b];
    }

    // 16.10 계단을 올라가는 방법의 개수 구하기
    int ComputeNumberOfWaysToH(int h, int maximum_step, std::vector<int> *number_of_ways_to_h_ptr);

    int NumberOfWaysToTop(int top, int maximum_step) {
        return ComputeNumberOfWaysToH(top, maximum_step,
                                      std::make_unique<std::vector<int>>(top + 1, 0).get());
    }

    int ComputeNumberOfWaysToH(int h, int maximum_step, std::vector<int> *number_of_ways_to_h_ptr) {
        if (h <= 1)
            return 1;

        std::vector<int> &number_of_ways_to_h = *number_of_ways_to_h_ptr;
        if (number_of_ways_to_h[h] == 0) {
            for (int i = 1; i <= maximum_step && h - i; ++i) {
                number_of_ways_to_h[h] +=
                        ComputeNumberOfWaysToH(h - i,
                                               maximum_step,
                                               number_of_ways_to_h_ptr);
            }
        }
        return number_of_ways_to_h[h];
    }

    // 16.11 텍스트를 예쁘게 배치하기
    int MinimumMessiness(const std::vector<std::string> &words,
                         int line_length) {
        //
        std::vector<int> minimum_messiness(std::size(words), std::numeric_limits<int>::max());
        int num_remaining_blanks = line_length - std::size(words[0]);
        minimum_messiness[0] = num_remaining_blanks * num_remaining_blanks;
        for (int i = 1; i < std::size(words); ++i) {
            num_remaining_blanks = line_length - std::size(words[i]);
            minimum_messiness[i] =
                    minimum_messiness[i - 1] + num_remaining_blanks * num_remaining_blanks;

            for (int j = i - 1; j >= 0; --j) {
                num_remaining_blanks -= (std::size(words[j]) + 1);
                if (num_remaining_blanks < 0)
                    break;
                int first_j_messiness = j - 1 < 0 ? 0 : minimum_messiness[j - 1];
                int current_line_messiness = num_remaining_blanks * num_remaining_blanks;
                minimum_messiness[i] =
                        std::min(minimum_messiness[i],
                                 first_j_messiness + current_line_messiness);
            }
        }
        return minimum_messiness.back();
    }

    // 16.2 감소하지 않는 가장 긴 부분 수열 찾기
    int LongestNondecreasingSubsequenceLength(const std::vector<int> &A) {
        //
        //
        std::vector<int> max_length(std::size(A), 1);
        for (int i = 1; i < std::size(A); ++i) {
            for (int j = 0; j < i; ++j) {
                if (A[i] >= A[j])
                    max_length[i] = std::max(max_length[i], max_length[j] + 1);
            }
        }
        return *std::max_element(std::begin(max_length), std::end(max_length));
    }

}

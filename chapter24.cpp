//
// Created by jewoo on 2021-11-24.
//
// 고난도 문제


#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

namespace ch_24 {
    long long Gcd(long long x, long long y) {
        if (x > y)
            return Gcd(y, x);
        else if (x == 0)
            return y;
        else if (!(x & 1) && !(y & 1))
            return Gcd(x >> 1, y >> 1) << 1;
        else if (!(x & 1) && y & 1)
            return Gcd(x >> 1, y);
        else if (x & 1 && !(y & 1))
            return Gcd(x, y >> 1);
        return Gcd(x, y - x);
    }

    unsigned int FindFirstMissingPositive(std::vector<int> A) {

        for (int i = 0; i < std::size(A); ++i) {
            while (0 < A[i] && A[i] <= std::size(A) && A[i] != A[A[i] - 1])
                std::swap(A[i], A[A[i] - 1]);

        }
        for (int i = 0; i < std::size(A); ++i) {
            if (A[i] != i + 1)
                return i + 1;
        }
        return std::size(A) + 1;
    }

    double UnlimitedPairsProfits(const std::vector<double> &prices);

    double BuyAndSellStockTimes(const std::vector<double> &prices, int k) {
        if (k == 0)
            return 0.0;
        else if (2 * k >= std::size(prices)) {
            return UnlimitedPairsProfits(prices);
        }
        std::vector<double> min_prices(k, std::numeric_limits<double>::infinity()), max_profit(k, 0.0);
        for (double price: prices) {
            for (int i = k - 1; i >= 0; --i) {
                max_profit[i] = std::max(max_profit[i], price - min_prices[i]);
                min_prices[i] = std::min(min_prices[i], price - (i ? max_profit[i - 1] : 0.0));
            }
        }
        return max_profit.back();
    }

    double UnlimitedPairsProfits(const std::vector<double> &prices) {
        double profit{0.0};
        for (int i = 0; i < std::size(prices); ++i) {
            profit += std::max(0.0, prices[i] - prices[i - 1]);
        }
        return profit;
    }

    int FindBiggestNMinusOneProduct(const std::vector<int> &A) {
        std::vector<int> suffix_products(std::size(A));

        partial_sum(std::crbegin(A), std::crend(A), std::rbegin(suffix_products));

        int prefix_product = 1, max_product = std::numeric_limits<int>::min();
        for (int i = 0; i < std::size(A); ++i) {
            int suffix_product = i + 1 < std::size(A) ? suffix_products[i + 1] : 1;
            max_product = std::max(max_product, prefix_product * suffix_product);
            prefix_product *= A[i];
        }
        return max_product;
    }

    int FindBiggestNMinusOneProduct2(const std::vector<int> &A) {
        int least_nonnegative_idx = -1;
        int number_of_negatives = 0, greatest_negative_idx = -1,
                least_negative_idx = -1;

        for (int i = 0; i < A.size(); ++i) {
            if (A[i] < 0) {
                ++number_of_negatives;
                if (least_negative_idx == -1 || A[least_negative_idx] < A[i])
                    least_negative_idx = i;

                if (greatest_negative_idx == -1 || A[i] < A[greatest_negative_idx])
                    greatest_negative_idx = i;
            } else {
                if (least_nonnegative_idx == -1 || A[i] < A[least_nonnegative_idx])
                    least_nonnegative_idx = i;
            }
        }
        int product{1};
        int idx_to_skip = number_of_negatives % 2 ? least_negative_idx
                                                  : (least_nonnegative_idx != -1 ? least_nonnegative_idx
                                                                                 : greatest_negative_idx);
        for (int i = 0; i < A.size(); ++i)
            if (i != idx_to_skip)
                product *= A[i];

        return product;
    }

    struct SubArray {
        int start = 0, end = 0;

        SubArray FindLongestIncreasingSubarray(const std::vector<int> &A) {

            int max_length = 1;
            SubArray result;
            int i = 0;
            while (i < std::size(A) - max_length) {
                //
                bool is_skippable = false;
                for (int j = i + max_length; j > i; --j) {
                    if (A[j - 1] >= A[j]) {
                        i = j;
                        is_skippable = true;
                        break;
                    }
                }
                //
                if (!is_skippable) {
                    i += max_length;
                    while (i < std::size(A) && A[i - 1] < A[i]) {
                        ++i, ++max_length;
                    }
                    result = {i - max_length, i - 1};
                }
            }
            return result;
        }
    };

    void ApplyCyclicPermutation(int rotate_amount, int offset, int cycle_length, std::vector<int> *A_ptr);

// 6. 배열 회전하기
    void RotateArray(int rotate_amount, std::vector<int> *A_ptr) {
        rotate_amount %= std::size(*A_ptr);
        if (!rotate_amount)
            return;

        int num_cycles = std::gcd(std::size(*A_ptr), rotate_amount);
        int cycle_length = std::size(*A_ptr) / num_cycles;
        for (int c = 0; c < num_cycles; ++c) {
            ApplyCyclicPermutation(rotate_amount, c, cycle_length, A_ptr);
        }
    }

    void ApplyCyclicPermutation(int rotate_amount, int offset, int cycle_length, std::vector<int> *A_ptr) {
        std::vector<int> &A = *A_ptr;

        int temp = A[offset];
        for (int i = 1; i < cycle_length; ++i) {
            std::swap(A[(offset + i * rotate_amount) % std::size(A)], temp);
        }
        A[offset] = temp;
    }

    void RotateArray2(int rotate_amount, std::vector<int> *A) {
        rotate_amount %= std::size(*A);
        std::reverse(std::begin(*A), std::end(*A));
        std::reverse(std::begin(*A), std::begin(*A) + rotate_amount);
        std::reverse(std::begin(*A) + rotate_amount, std::end(*A));
    }

    // 7. 룩에게 공격당하는 위치 판별하기
    void RookAttack(std::vector<std::vector<int>> *A_ptr) {
        std::vector<std::vector<int>> &A = *A_ptr;

        int m = std::size(A), n = std::size(A[0]);
        bool has_first_row_zero = std::find(std::begin(A[0]), std::end(A[0]), 0) != std::end(A[0]);
        bool has_first_column_zero = std::any_of(
                std::begin(A), std::end(A), [](const std::vector<int> &row) {
                    return row[0] == 0;
                });
        for (int i = 1; i < m; ++i) {
            for (int j = 1; j < n; ++j) {
                if (!A[i][j])
                    A[i][0] = A[0][j] = 0;
            }
        }
        for (int i = 1; i < m; ++i) {
            if (!A[i][0])
                std::fill(std::next(std::begin(A[i])), std::end(A[i]), 0);
        }
        for (int i = 1; i < n; ++i) {
            if (!A[0][i])
                std::for_each(std::next(std::begin(A)),
                              std::end(A), [i](std::vector<int> &row) { row[i] = 0; });
        }
        if (has_first_row_zero)
            std::fill(std::begin(A[0]), std::end(A[0]), 0);

        if (has_first_column_zero)
            std::for_each(std::begin(A), std::end(A),
                          [](std::vector<int> &row) { row[0] = 0; });
    }

    // 8. 텍스트 줄 맞추기
    std::string Join(const std::vector<std::string> &words, const std::string &delimiter);

    std::vector<std::string> JustifyText(const std::vector<std::string> &words, int L) {
        int curr_line_length = 0;
        std::vector<std::string> result, curr_line;

        for (const std::string &word: words) {
            if (curr_line_length + std::size(word) + std::size(curr_line) > L) {
                for (int i = 0; i < L - curr_line_length; ++i) {
                    curr_line[i % std::max(static_cast<int>(std::size(curr_line)) - 1, 1)] += ' ';
                }
                result.emplace_back(Join(curr_line, ""));
                curr_line.clear();
                curr_line_length = 0;
            }
            curr_line.emplace_back(word);
            curr_line_length += std::size(word);
        }
        // 마지막 줄 처리하기, 마지막 줄은 왼쪽으로 정렬되어 있다.
        result.emplace_back(Join(curr_line, " ")
                                    .append(L - curr_line_length - (std::size(curr_line) - 1), ' '));
        return result;
    }

    std::string Join(const std::vector<std::string> &words, const std::string &delimiter) {
        return std::accumulate(std::next(std::begin(words)), std::end(words), words.front(),
                               [delimiter](const std::string &result, const std::string &word) {
                                   return result + delimiter + word;
                               });
    }
    // 9. 리스트 지핑 구하기

}

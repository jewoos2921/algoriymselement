//
// Created by jewoo on 2021-11-20.
//
// 배열 부트 캠프

#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <numeric>
#include <random>
#include <unordered_map>



// 정수 배열이 주어졌을 떄 짝수가 가장 먼저 나오게 하기


void EvenOdd(std::vector<int> *A_ptr) {
    std::vector<int> &A = *A_ptr;
    int next_even = 0, next_odd = size(A) - 1;

    while (next_even < next_odd) {
        if (A[next_even] % 2 == 0) {
            ++next_even;
        } else {
            std::swap(A[next_even], A[next_odd--]);
        }
    }
}

// 5.1 네덜란드 국기 문제
namespace {
    typedef enum {
        kRed,
        kWhite,
        kBlue
    } Color;

    void DutchFlagPartition(int pivot_index, std::vector<Color> *A_ptr) {
        std::vector<Color> &A = *A_ptr;
        Color pivot = A[pivot_index];

        //
        for (int i = 0; i < size(A); ++i) {

            for (int j = i + 1; j < size(A); ++j) {
                if (A[j] < pivot) {
                    std::swap(A[i], A[j]);
                    break;
                }

            }
        }
        for (int i = size(A) - 1; i >= 0; --i) {

            for (int j = i - 1; j >= 0; --j) {
                if (A[j] > pivot) {
                    std::swap(A[i], A[j]);
                    break;
                }
            }
        }
    }

    void DutchFlagPartition2(int pivot_index, std::vector<Color> *A_ptr) {
        std::vector<Color> &A = *A_ptr;
        Color pivot = A[pivot_index];

        int smaller = 0;
        for (int i = 0; i < size(A); ++i) {
            if (A[i] < pivot)
                std::swap(A[i], A[smaller++]);
        }
        int largest = size(A) - 1;

        for (int i = size(A) - 1; i >= 0; --i) {
            if (A[i] > pivot)
                std::swap(A[i], A[largest--]);
        }
    }

    void DutchFlagPartition3(int pivot_index, std::vector<Color> *A_ptr) {
        std::vector<Color> &A = *A_ptr;
        Color pivot = A[pivot_index];


        int smaller = 0, equal = 0, larger = size(A);

        while (equal < larger) {

            if (A[equal] < pivot)
                std::swap(A[smaller++], A[equal++]);
            else if (A[equal] == pivot)
                ++equal;
            else
                std::swap(A[equal], A[--larger]);
        }
    }
}

// 5.2 임의의 정수값 증가하기
std::vector<int> PlusOne(std::vector<int> A) {
    ++A.back();
    for (int i = std::size(A) - 1; i > 0 && A[i] == 10; --i) {
        A[i] = 0, ++A[i - 1];
    }
    if (A[0] == 10) {
        A[0] = 1;
        A.emplace_back(0);
    }
    return A;
}

// 5.3 임의의 두정수 값 곱하기
std::vector<int> Multiply(std::vector<int> num1, std::vector<int> num2) {
    const int sign = (num1.front() < 0) ^ (num2.front() < 0) ? -1 : 1;
    num1.front() = abs(num1.front()), num2.front() = abs(num2.front());

    std::vector<int> result(std::size(num1) + std::size(num2), 0);

    for (int i = std::size(num1) - 1; i >= 0; --i) {
        for (int j = std::size(num2) - 1; j >= 0; --j) {
            result[i + j + 1] += num1[i] * num2[j];
            result[i + j] += result[i + j + 1] / 10;
            result[i + j + 1] %= 10;
        }
    }

    // 0으로 시작하는 부분을 제거한다
    result = {
            std::ranges::find_if_not(std::begin(result), std::end(result), [](int a) { return a == 0; }),
            std::end(result)};
    if (std::empty(result))
        return {0};

    result.front() *= sign;
    return result;
}

// 5.4 배열에서 이동하기
bool CanReachEnd(const std::vector<int> &max_advanced_steps) {
    int furthest_reach_so_far = 0, last_index = std::size(max_advanced_steps) - 1;
    for (int i = 0; i <= furthest_reach_so_far && furthest_reach_so_far < last_index; ++i) {
        furthest_reach_so_far = std::max(furthest_reach_so_far, max_advanced_steps[i] + i);
    }
    return furthest_reach_so_far >= last_index;
}

// 5.5 정렬된 배열에서 중복 제거하기
int DeleteDuplicates(std::vector<int> *A_ptr) {
    std::vector<int> &A = *A_ptr;

    if (std::empty(A))
        return 0;

    int write_index{1};
    for (int i = 1; i < std::size(A); ++i) {
        if (A[write_index - 1] != A[i])
            A[write_index++] = A[i];
    }
    return write_index;
}

// 5.6 주식 한번 사고 팔기
double BuyAndSellStockOnce(const std::vector<double> &prices) {
    double min_price_so_far = std::numeric_limits<double>::infinity(), max_profit = 0;
    for (double price: prices) {
        double max_profit_sell_today = price - min_price_so_far;
        max_profit = std::max(max_profit, max_profit_sell_today);
        min_price_so_far = std::min(min_price_so_far, price);
    }
    return max_profit;
}

// 5.7 주식 두번 사고 팔기
double BuyAndSellStockTwice(const std::vector<double> &prices) {
    double max_total_profit = 0;

    std::vector<double> first_buy_sell_profits(std::size(prices), 0);
    double min_price_so_far = std::numeric_limits<double>::infinity();

    //
    //
    for (int i = 0; i < std::size(prices); ++i) {
        min_price_so_far = std::min(min_price_so_far, prices[i]);
        max_total_profit = std::max(max_total_profit, prices[i] - min_price_so_far);
        first_buy_sell_profits[i] = max_total_profit;
    }

    //
    //
    double max_price_so_far = std::numeric_limits<double>::min();
    for (int i = std::size(prices) - 1; i > 0; --i) {
        max_price_so_far = std::max(max_price_so_far, prices[i]);
        max_total_profit = std::max(max_total_profit, max_price_so_far - prices[i] + first_buy_sell_profits[i]);
    }
    return max_total_profit;
}

// 5.8 대체 연산
void Rearrange(std::vector<int> *A_ptr) {
    std::vector<int> &A = *A_ptr;
    for (size_t i = 1; i < std::size(A); ++i)
        if ((!(i % 2) && A[i - 1] < A[i]) || ((i % 2) && A[i - 1] > A[i])) {
            std::swap(A[i - 1], A[i]);
        }
}

// 5.9 n보다 작은 모든 소수 나열하기
std::vector<int> GeneratePrimes(int n) {
    std::vector<int> primes;

    std::deque<bool> is_prime(n + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int p = 2; p <= n; ++p) {
        if (is_prime[p]) {
            primes.emplace_back(p);

            for (int i = p * 2; i <= n; i += p) {
                is_prime[i] = false;
            }
        }
    }
    return primes;
}

std::vector<int> GeneratePrimes2(int n) {
    if (n < 2)
        return {};

    const int size = floor(0.5 * (n - 3)) + 1;
    std::vector<int> primes;
    primes.emplace_back(2);
    //
    //
    //
    //
    std::deque<bool> is_prime(size, true);
    for (int i = 0; i < size; ++i) {
        if (is_prime[i]) {
            int p = (i * 2) + 3;
            primes.emplace_back(p);
            //
            //
            //
            //
            //
            for (long long j = 2LL * i * i + 6 * i + 3; j < size; j += p) {
                is_prime[j] = false;
            }
        }
    }
    return primes;
}
// 5.10 배열 안의 원소로 수열 구하기

void ApplyPermutation(std::vector<int> perm, std::vector<int> *A_ptr) {
    std::vector<int> &A = *A_ptr;
    for (int i = 0; i < std::size(A); ++i) {
        while (perm[i] != i) {
            std::swap(A[i], A[perm[i]]);
            std::swap(perm[i], perm[perm[i]]);
        }
    }
}

// 5.11 다음 순열 구하기
std::vector<int> NextPermutation(std::vector<int> perm) {
    auto inversion_point = std::is_sorted_until(std::rbegin(perm), std::rend(perm));
    if (inversion_point == std::rend(perm)) {
        return {};
    }

    auto least_upper_bound = std::upper_bound(std::rbegin(perm), inversion_point, *inversion_point);

    std::iter_swap(inversion_point, least_upper_bound);

    std::reverse(std::rbegin(perm), inversion_point);
    return perm;
}

// 5.12 오프라인 데이터 샘플 구하기
void RandomSampling(int k, std::vector<int> *A_ptr) {
    std::vector<int> &A = *A_ptr;

    std::default_random_engine seed((std::random_device()) ()); // 난수 생성기
    for (int i = 0; i < k; ++i) {
        std::swap(A[i], A[std::uniform_int_distribution<int>{i, static_cast<int>(A.size()) - 1}(seed)]);
    }
}

// 5.13 온라인 데이터 샘플 구하기
// 가정: 적어도 k개의 원소가 유입된다.
std::vector<int> OnlineRandomSample(std::vector<int>::const_iterator stream_begin,
                                    const std::vector<int>::const_iterator steam_end, int k) {

    std::vector<int> running_sample;
    // 각 k개의 원소를 저장한다.
    for (int i = 0; i < k; ++i) {
        running_sample.emplace_back(*stream_begin++);
    }

    std::default_random_engine seed((std::random_device()) ()); // 난수 생성기
    int num_seen_so_far = k;
    while (stream_begin != steam_end) {
        int x = *stream_begin++;
        ++num_seen_so_far;
        // [0, num_seen_so_far -1] 사이에서 임의의 숫자를 생성한다,
        // 그리고 만약에 그 숫자가 [0, k -1] 사이에 들어 있다면, 해당 원소를
        // x와 맞바꾼다.
        if (const int idx_to_replace = std::uniform_int_distribution<int>{0, num_seen_so_far - 1}(seed);
                idx_to_replace < k) {
            running_sample[idx_to_replace] = x;
        }
    }
    return running_sample;
}

// 5.14 임의의 순열 계산하기
std::vector<int> ComputeRandomPermutation(int n) {
    std::vector<int> permutation(n);

    std::iota(std::begin(permutation), std::end(permutation), 0);
    RandomSampling(n, &permutation);
    return permutation;
}

// 5.15 임의의 부분 집합 만들기
std::vector<int> RandomSubset(int n, int k) {
    std::unordered_map<int, int> changed_elements;

    std::default_random_engine seed((std::random_device()) ()); // 난수 생성기

    for (int i = 0; i < k; ++i) {

        int rand_idx = std::uniform_int_distribution<int>{i, n - 1}(seed);
        if (auto ptr1 = changed_elements.find(rand_idx), ptr2 = changed_elements.find(i);
                ptr1 == std::end(changed_elements) && ptr2 == std::end(changed_elements)) {
            changed_elements[rand_idx] = i;
            changed_elements[i] = rand_idx;
        } else if (ptr1 == std::end(changed_elements) && ptr2 != std::end(changed_elements)) {
            changed_elements[rand_idx] = ptr2->second;
            ptr2->second = rand_idx;
        } else if (ptr1 == std::end(changed_elements) && ptr2 == std::end(changed_elements)) {
            changed_elements[rand_idx] = ptr1->second;
            ptr1->second = i;
        } else {
            int temp = ptr2->second;
            changed_elements[i] = ptr1->second;
            changed_elements[rand_idx] = temp;
        }
    }
    std::vector<int> result;
    for (int i = 0; i < k; ++i) {
        result.emplace_back(changed_elements[i]);
    }
    return result;
}

// 5.16 균등하지 않은 임의의 숫자 생성하기
int NonUniformRandomNumberGeneration(const std::vector<int> &values,
                                     const std::vector<double> &probabilities) {

    std::vector<double> prefix_sum_of_probabilities;

    std::partial_sum(std::cbegin(probabilities), std::cend(probabilities),
                     std::back_inserter(prefix_sum_of_probabilities));

    std::default_random_engine seed((std::random_device()) ()); // 난수 생성기

    const double uniform_0_1 = std::generate_canonical<double, std::numeric_limits<double>::digits>(seed);

    const int interval_idx =
            std::distance(std::cbegin(prefix_sum_of_probabilities),
                          std::upper_bound(std::cbegin(prefix_sum_of_probabilities),
                                           std::cend(prefix_sum_of_probabilities), uniform_0_1));
    return values[interval_idx];
}

// 5.17 스도쿠 체크
namespace {

    bool
    HasDuplicate(const std::vector<std::vector<int>> &partial_assignment, int start_row, int end_row, int start_col,
                 int end_col);

    bool IsValidSudoku(const std::vector<std::vector<int>> &partial_assignment) {
        for (int i = 0; i < std::size(partial_assignment); ++i) {
            if (HasDuplicate(partial_assignment, i, i + 1, 0, std::size(partial_assignment)))
                return false;
        }
        for (int j = 0; j < std::size(partial_assignment); ++j) {
            if (HasDuplicate(partial_assignment, 0, std::size(partial_assignment), j, j + 1))
                return false;
        }
        int region_size = std::sqrt(std::size(partial_assignment));
        for (int I = 0; I < region_size; ++I) {
            for (int J = 0; J < region_size; ++J) {
                if (HasDuplicate(partial_assignment, region_size * I,
                                 region_size * (I + 1), region_size * J,
                                 region_size * (J + 1)))
                    return false;
            }
        }
        return true;
    }

    bool
    HasDuplicate(const std::vector<std::vector<int>> &partial_assignment, int start_row, int end_row, int start_col,
                 int end_col) {
        std::deque<bool> is_present(std::size(partial_assignment) + 1, false);
        for (int i = start_row; i < end_row; ++i) {
            for (int j = start_col; j < end_col; ++j) {
                if (partial_assignment[i][j] != 0 && is_present[partial_assignment[i][j]])
                    return true;
                is_present[partial_assignment[i][j]] = true;
            }
        }
        return false;
    }

}
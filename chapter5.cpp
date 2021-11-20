//
// Created by jewoo on 2021-11-20.
//
// 배열 부트 캠프

#include <iostream>
#include <vector>

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

}

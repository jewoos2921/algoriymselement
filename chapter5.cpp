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
            swap(A[next_even], A[next_odd--]);
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

namespace {
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
}
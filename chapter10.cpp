//
// Created by jewoo on 2021-11-30.
//
// 힙

#include <vector>
#include <queue>
#include <functional>

namespace ch10 {
    std::vector<std::string> TopK(int k,
                                  std::vector<std::string>::const_iterator stream_begin,
                                  const std::vector<std::string>::const_iterator &stream_end) {
        std::priority_queue<std::string,
                std::vector<std::string>, std::function<bool(std::string, std::string) >> min_heap(
                [](const std::string &a, const std::string &b) {
                    return std::size(a) >= std::size(b);
                });

        while (stream_begin != stream_end) {
            min_heap.emplace(*stream_begin);
            if (std::size(min_heap) > k) {
                // 길이가 가장 작은 문자열을 삭제한다.
                // 앞의 비교 함수를 통해 문자열을 길이 순서대로 나열할 수 있음을 기억하라.
                min_heap.pop();
            }
            stream_begin = std::next(stream_begin);
        }
        std::vector<std::string> result;
        while (!std::empty(min_heap)) {
            result.emplace_back(min_heap.top());
            min_heap.pop();
        }
        return result;
    }

    // 10.1 정렬된 파일 합치기
    struct IteratorCurrentAndEnd {
        bool operator>(const IteratorCurrentAndEnd &that) const {
            return *current > *that.current;
        }

        std::vector<int>::const_iterator current;
        std::vector<int>::const_iterator end;
    };

    std::vector<int> MergeSortedArrays(const std::vector<std::vector<int>> &sorted_arrays) {
        std::priority_queue<IteratorCurrentAndEnd, std::vector<IteratorCurrentAndEnd>, std::greater<>> min_heap;

        for (const std::vector<int> &sorted_array: sorted_arrays) {
            if (!std::empty(sorted_array))
                min_heap.emplace(
                        IteratorCurrentAndEnd{std::cbegin(sorted_array), std::cend(sorted_array)});
        }
        std::vector<int> result;

        while (!std::empty(min_heap)) {
            IteratorCurrentAndEnd smallest_array = min_heap.top();
            min_heap.pop();
            result.emplace_back(*smallest_array.current);
            if (std::next(smallest_array.current) != smallest_array.end) {
                min_heap.emplace(IteratorCurrentAndEnd{
                        std::next(smallest_array.current), smallest_array.end});
            }
        }
        return result;
    }

// 10.2 증가했다가 감소하는 배열 정렬하기

    std::vector<int> SortKIncreasingDecreasingArray(const std::vector<int> &A) {
        // A를 정렬된 배열의 집합으로 구분한다.
        std::vector<std::vector<int>> sorted_subarrays;
        enum class SubarrayType {
            kIncreasing,
            kDecreasing
        } subarray_type = SubarrayType::kIncreasing;

        int start_idx = 0;
        for (int i = 1; i <= std::size(A); ++i) {
            if (i == std::size(A) || // A가 끝났다. 마지막 부분 배열을 추가한다.
                (A[i - 1] < A[i] && subarray_type == SubarrayType::kDecreasing) ||
                (A[i - 1] >= A[i] && subarray_type == SubarrayType::kIncreasing)) {

                if (subarray_type == SubarrayType::kIncreasing) {
                    sorted_subarrays.emplace_back(std::cbegin(A) + start_idx, std::cbegin(A) + i);
                } else {
                    sorted_subarrays.emplace_back(std::crbegin(A) + std::size(A) - i,
                                                  std::crbegin(A) + std::size(A) - start_idx);
                }
                start_idx = i;
                subarray_type = subarray_type == SubarrayType::kIncreasing ? SubarrayType::kDecreasing
                                                                           : SubarrayType::kIncreasing;
            }
        }
        return MergeSortedArrays(sorted_subarrays);
    }

    // 10.3 거의 정렬된 배열 정렬하기
    std::vector<int> SortApproximatelySortedData(
            std::vector<int>::const_iterator sequence_begin,
            const std::vector<int>::const_iterator &sequence_end, int k) {

        std::priority_queue<int, std::vector<int>, std::greater<>> min_heap;

        // 첫 k개의 원소를 min_heap 에 추가한다
        // 만약 k보다 적은 원소가 있다면 멈춘다.
        for (int i = 0; i < k && sequence_begin != sequence_end; ++i) {
            min_heap.push(*sequence_begin++);
        }

        std::vector<int> result;
        // 새로운 원소가 들어오면, min_heap 에 추가한 뒤 최솟값을 뽑아낸다.
        while (sequence_begin != sequence_end) {
            min_heap.push(*sequence_begin++);
            result.push_back(min_heap.top());
            min_heap.pop();
        }

        // 원소를 모두 읽었다면, 반복적으로 남아 있는 원소를 뽑아낸다.
        while (!std::empty(min_heap)) {
            result.push_back(min_heap.top());
            min_heap.pop();
        }
        return result;
    }

    // 10.4 k개의 가까운 별 찾기
    struct Star {
        bool operator<(const Star &that) const {
            return Distance() < that.Distance();
        }

        double Distance() const { return sqrt(x * x + y * y + z * z); }

        double x, y, z;
    };

    std::vector<Star> FindClosestKStars(std::vector<Star>::const_iterator stars_begin,
                                        const std::vector<Star>::const_iterator &stars_end,
                                        int k) {
        // 현재까지 중에서 가장 가까운 별 k개를 저장할 max_heap
        std::priority_queue<Star> max_heap;

        while (stars_begin != stars_end) {
            // 각각의 별을 최대힙에 추가한다. 만약 최대힙의 크기가 k보다 커진다면,
            // 최대힙에서 최대 원소를 삭제한다.
            max_heap.emplace(*stars_begin++);
            if (std::size(max_heap) == k + 1)
                max_heap.pop();
        }

        // 최대힙에서 반복적으로 추출하여 가장 먼 별부터 가장 가까운
        // 별 순서대로 정렬한다.
        std::vector<Star> closest_stars;
        while (!std::empty(max_heap)) {
            closest_stars.emplace_back(max_heap.top());
            max_heap.pop();
        }

        return {std::rbegin(closest_stars), std::rend(closest_stars)};
    }

    // 10.5 실시간으로 주어진 데이터의 중앙값 찾기
    std::vector<double> OnlineMedian(std::vector<int>::const_iterator sequence_begin,
                                     const std::vector<int>::const_iterator &sequence_end) {
        // min_heap 은 현재까지의 값이 큰 절반을 저장한다.
        std::priority_queue<int, std::vector<int>, std::greater<>> min_heap;
        // max_heap 은 현재까지의 값이 작은 절반을 저장한다.
        std::priority_queue<int, std::vector<int>, std::less<>> max_heap;

        std::vector<double> result;

        while (sequence_begin != sequence_end) {
            min_heap.emplace(*sequence_begin++);
            max_heap.emplace(min_heap.top());
            min_heap.pop();

            // 짝수 개의 원소를 읽었다면 min_heap 과 max_heap 에는 동일한 개수의 원소가
            // 들어 있어야 한다. 홀수 개의 원소를 읽어다면, min_heap 이 언제나 max_heap 보다
            // 원소 1개를 더 많이 가져야 한다.

            if (std::size(max_heap) > std::size(min_heap)) {
                min_heap.emplace(max_heap.top());
                max_heap.pop();
            }

            result.emplace_back(std::size(min_heap) == std::size(max_heap) ? 0.5 * (min_heap.top() + max_heap.top())
                                                                           : min_heap.top());
        }
        return result;
    }

    // 10.6 최대힙에서 가장 큰 원소 k개 구하기
    std::vector<int> KLargestInBinaryHeap(const std::vector<int> &A, int k) {
        if (k <= 0)
            return {};

        struct HeapEntry {
            int index, value;
        };
        std::priority_queue<HeapEntry, std::vector<HeapEntry>,
                std::function<bool(HeapEntry, HeapEntry)>> candidate_max_heap(
                [](const HeapEntry &a, const HeapEntry &b) {
                    return a.value < b.value;
                });

        // A에서 가장 큰 원소는 인덱스 0에 있다.
        candidate_max_heap.emplace(HeapEntry{0, A[0]});
        std::vector<int> result;
        for (int i = 0; i < k; ++i) {
            int candidate_idx = candidate_max_heap.top().index;
            result.emplace_back(candidate_max_heap.top().value);
            candidate_max_heap.pop();

            if (int left_child_idx = 2 * candidate_idx + 1; left_child_idx < std::size(A)) {
                candidate_max_heap.emplace(HeapEntry{left_child_idx, A[left_child_idx]});
            }
            if (int right_child_idx = 2 * candidate_idx + 2;
                    right_child_idx < std::size(A)) {
                candidate_max_heap.emplace(HeapEntry{right_child_idx, A[right_child_idx]});
            }
        }
        return result;
    }
}


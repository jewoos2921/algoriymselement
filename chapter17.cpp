//
// Created by jewoo on 2021-11-25.
//
// 그리디 알고리즘과 불변식
#include <array>
#include <iostream>
#include <vector>
#include <algorithm>

namespace ch_17 {
    int ChangeMaking(int cents) {
        const std::array<int, 6> kCoints = {100, 50, 25, 10, 5, 1};
        int num_coins = 0;
        for (int coin: kCoints) {
            num_coins += cents / coin;
            cents %= coin;
        }
        return num_coins;
    }

    // 1. 최적의 업무 할당 구하기
    struct PairedTasks {
        int task_1, task_2;
    };

    std::vector<PairedTasks> OptimumTaskAssignment(std::vector<int> task_durations) {
        std::sort(std::begin(task_durations), std::end(task_durations));
        std::vector<PairedTasks> optimum_assignments;
        for (int i = 0, j = std::size(task_durations) - 1; i < j; ++i, --j) {
            optimum_assignments.emplace_back(
                    PairedTasks{task_durations[i], task_durations[j]}
            );
        }
        return optimum_assignments;
    }

    // 2. 기다리는 시간을 최소화 하기
    int MinimumTotalWaitingTime(std::vector<int> service_times) {
        std::sort(std::begin(service_times), std::end(service_times));

        int total_waiting_time = 0;
        for (int i = 0; i < std::size(service_times); ++i) {
            int num_remaining_queries = std::size(service_times) - (i + 1);
            total_waiting_time += service_times[i] * num_remaining_queries;
        }
        return total_waiting_time;
    }

    // 3. 모든 구간을 커버하기
    struct Interval {
        int left, right;
    };

    int FindMinimumVisits(std::vector<Interval> intervals) {
        std::sort(std::begin(intervals),
                  std::end(intervals), [](const Interval &a, const Interval &b) { return a.right < b.right; });
        int last_visit_time = std::numeric_limits<int>::min(), num_visits = 0;
        for (const Interval &interval: intervals) {
            if (interval.left > last_visit_time) {
                last_visit_time = interval.right;
                ++num_visits;
            }
        }
        return num_visits;
    }

    // 불변식 부트 캠프
    bool HasTwoSum(const std::vector<int> &A, int t) {
        int i = 0, j = std::size(A) - 1;
        while (i <= j) {
            if (A[i] + A[j] == t)
                return true;
            else if (A[i] + A[j] < t)
                ++i;
            else // A[i] + A[j] > t.
                --j;
        }
        return false;
    }

    //
    bool HasThreeSum(std::vector<int> A, int t) {
        return std::any_of(std::begin(A),
                           std::end(A),
                           [&](int a) { return HasTwoSum(A, t - a); });
    }

    // 5. 다수 원소 찾기
    std::string MajoritySearch(std::vector<std::string>::const_iterator stream_begin,
                               const std::vector<std::string>::const_iterator stream_end) {
        std::string candidate;
        int candidate_count = 0;
        while (stream_begin != stream_end) {
            std::string it = *stream_begin++;
            if (candidate_count == 0) {
                candidate = it;
                candidate_count = 1;
            } else if (candidate == it) {
                ++candidate_count;
            } else {
                --candidate_count;
            }
        }
        return candidate;
    }

    // 6. 주유소 문제


    int FindAmpleCity(const std::vector<int> &gallons,
                      const std::vector<int> &distances) {

        const int kMPG = 20;

        int remaining_gallons = 0;

        struct CityAndRemainingGas {
            int city = 0, remaining_gallons = 0;
        };

        CityAndRemainingGas city_remaining_gallons;
        const int num_cities = std::size(gallons);

        for (int i = 1; i < num_cities; ++i) {
            remaining_gallons += gallons[i - 1] - distances[i - 1] / kMPG;
            if (remaining_gallons < city_remaining_gallons.remaining_gallons)
                city_remaining_gallons = {i, remaining_gallons};
        }
        return city_remaining_gallons.city;
    }

}


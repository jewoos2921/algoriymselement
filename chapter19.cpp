//
// Created by jewoo on 2021-11-19.
//

// 병렬 컴퓨팅
// 고갈: 자원이 필요하지만 확보할 수 없을 때
// 데드락: A 스레드가 락 L1을 취득하고 B 스레드가 락 L2를 취득한 상태에서 A는 L2를 취득하려고 하고, B는 L1을 취득하려고 할 때
// 라이브락: 프로세서가 어떤 연산을 반복 실행하지만 계속 실패할 때

#include <mutex>
#include <iostream>
#include <asio/asio.hpp>

using namespace std;

class Semaphore {
public:
    Semaphore(int max_available) : max_available_(max_available), taken_(0) {}

    void Acquire() {
        unique_lock<mutex> lock(mx_);
        while (taken_ == max_available_) {
            cond_.wait(lock);
        }
        ++taken_;
    }

    void Release() {
        lock_guard<mutex> lock(mx_);
        --taken_;
        cond_.notify_all();
    }

private:
    int max_available_;
    int taken_;
    mutex mx_;
    condition_variable cond_;
};

// 19.1 다중 스레드 사전을 위한 테시 구현하기
// 실시간으로 오타를 정정해 주는 프로그램
// 클라이언트가 문자열을 전송하면 사전에서 입력 문자열과 가장 가까운 문자열을 찾아 문자열 배열을 반환해야 한다.
namespace {
    class SpellCheckService {
    public:
        static void Service(ServiceRequest &req, ServiceResponse &resp) {
            if (string w = req.ExtractWordToCheckFromRequest(); w != w_last_) {
                w_last_ = move(w);
                closest_to_last_word_ = ClosestInDictionary(w_last_);
            }
            resp.EncodeIntoResponse(closest_to_last_word_);
        }

    private:
        static string w_last_;
        static vector<string> closest_to_last_word_;
    };

    // 해결
    // 다중 스레드가 동시에 가장 가까운 문자열들을 구하는 코드
    // 요청 처리에 걸리는 시간을 줄이기 위해 꼭 필요한 곳에만 락을 설정
    // 캐시를 사용하는 이유 역시 처리 시간을 줄이기 위해
    // 락을 사용하기 때문에 캐시를 읽거나 갱신하는 작업은 단일 작업이 된다.
    class SpellCheckService2 {
    public:
        static void Service(ServiceRequest &req, ServiceResponse &resp) {
            string w = req.ExtractWordToCheckFromRequest();
            vector<string> result;

            bool found = false;
            {
                lock_guard<mutex> lock(mx_);
                if (w == w_last_) {
                    result = closest_to_last_word_;
                    found = true;
                }
            }
            if (!found) {
                result = ClosestInDictionary(w);
                lock_guard<mutex> lock(mx_);
                w_last_ = move(w);
                closest_to_last_word_ = result;
            }
            resp.EncodeIntoResponse(closest_to_last_word_);
        }

    private:
        static string w_last_;
        static vector<string> closest_to_last_word_;
        static mutex mx_;
    };

    // 19.2 두 개의 스레드가 동기화되지 않은 채 번갈아 수행되는 경우 분석하기
    static int counter = 0;

    void IncrementThread(int n) {
        for (int i = 0; i < n; ++i) {
            ++counter;
        }
    }

    void TwoThreadIncrementDriver(int n) {
        thread T1(IncrementThread, n);
        thread T2(IncrementThread, n);
        T1.join();
        T2.join();
        cout << counter << endl;
    }

    // 19.3 두 개의 스레드가 번갈아 수행될 때 동기화 구현하기
    class OddEvenMonitor {
    public:
        static const bool ODD_TURN = true;
        static const bool EVEN_TURN = false;

        OddEvenMonitor() : turn_(ODD_TURN) {}

    private:
        bool turn_;
        condition_variable cond_;
        mutex mx_;

    public:
        void WaitTurn(bool old_turn) {
            unique_lock<mutex> lock(mx_);
            while (turn_ != old_turn) {
                // wait를 호출하면 알림 받은 현재 스레드의 실행을 차단
                cond_.wait(lock);
            }
        }

        void ToggleTrun() {
            lock_guard<mutex> lock(mx_);
            turn_ = !turn_;
            // notify_one을 호출하면 이 조건을 기다리는 스레드 중 하나를 차단 해제한다.
            cond_.notify_one();
        }
    };

    void OddThread(OddEvenMonitor &monitor) {
        for (int i = 1; i <= 100; i += 2) {
            monitor.WaitTurn(OddEvenMonitor::ODD_TURN);
            cout << i << endl;
            monitor.ToggleTrun();
        }
    }

    void EvenThread(OddEvenMonitor &monitor) {
        for (int i = 2; i <= 100; i += 2) {
            monitor.WaitTurn(OddEvenMonitor::EVEN_TURN);
            cout << i << endl;
            monitor.ToggleTrun();
        }
    }

    // 19.4 스레드 풀 구현하기
    // 간단한 HTTP 서버 구현
    const unsigned short SERVERPORT = 8080;

    void make_server(int argc, char *argv) {
        asio::io_service io_service;

    }

}
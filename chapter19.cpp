//
// Created by jewoo on 2021-11-19.
//

// 병렬 컴퓨팅
// 고갈: 자원이 필요하지만 확보할 수 없을 때
// 데드락: A 스레드가 락 L1을 취득하고 B 스레드가 락 L2를 취득한 상태에서 A는 L2를 취득하려고 하고, B는 L1을 취득하려고 할 때
// 라이브락: 프로세서가 어떤 연산을 반복 실행하지만 계속 실패할 때

#include <mutex>

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
}
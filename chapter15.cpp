//
// Created by jewoo on 2021-11-20.
//
// 재귀
#include <iostream>
#include <vector>
#include <array>
#include <stack>
#include <memory>

// 재귀 부트 캠프
long long GCD(long long x, long long y) {
    return y == 0 ? x : GCD(y, x % y);
}

// 15.1 하노이 타워 문제
namespace problem_15_1 {
    using namespace std;

    const int kNumPegs = 3;

    void
    ComputeTowerHanoiSteps(int num_rigs_to_move, array<stack<int>, kNumPegs> &pegs, int from_reg, int to_peg,
                           int use_peg,
                           vector<vector<int>> *result_ptr);

    vector<vector<int>> ComputeTowerHanoi(int num_rings) {
        array<stack<int>, kNumPegs> pegs;
        // 말뚝의 상태를 초기화한다.
        for (int i = num_rings; i >= 1; --i) {
            pegs[0].push(i);
        }

        vector<vector<int>> result;
        ComputeTowerHanoiSteps(num_rings, pegs, 0, 1, 2, &result);
        return result;
    }

    void
    ComputeTowerHanoiSteps(int num_rigs_to_move, array<stack<int>, kNumPegs> &pegs, int from_reg, int to_peg,
                           int use_peg,
                           vector<vector<int>> *result_ptr) {
        if (num_rigs_to_move > 0) {
            ComputeTowerHanoiSteps(num_rigs_to_move - 1,
                                   pegs, from_reg, use_peg, to_peg, result_ptr);
            pegs[to_peg].push(pegs[from_reg].top());
            pegs[from_reg].pop();
            result_ptr->emplace_back(vector<int>{from_reg, to_peg});
            ComputeTowerHanoiSteps(num_rigs_to_move - 1,
                                   pegs,
                                   use_peg, to_peg, from_reg, result_ptr);
        }
    }
}

// 2. 전화번호에 대한 모든 연상 기호 계산하기
namespace problem_15_2 {
    using namespace std;

    void
    PhoneMnemonicHelper(const string &phone_number, int digit, string *partial_mnemonic, vector<string> *mnemonics);

    vector<string> PhoneMnemonic(const string &phone_number) {
        vector<string> mnemonics;
        PhoneMnemonicHelper(phone_number,
                            0,
                            make_unique<string>(size(phone_number), 0).get(), &mnemonics);
        return mnemonics;
    }

    const int kNumTelDigits = 10;

    // 각 숫자에 매칭되는 문자 집합
    const array<string, kNumTelDigits> kMapping = {
            {"0", "1", "ABC",
             "DEF", "GHI", "JKL",
             "MNO", "PQRS", "TUV", "WXYZ"}
    };

    void
    PhoneMnemonicHelper(const string &phone_number, int digit, string *partial_mnemonic, vector<string> *mnemonics) {
        if (digit == size(phone_number)) {
            // 모든 숫자가 처리되므로, partial_mnemonic 을
            // mnemonics 에 추가
            // 이후 호출은 partial_mnemonic 을 수정하므로 복사본을 추가한다.
            mnemonics->emplace_back(*partial_mnemonic);
        } else {
            // 숫자에 매칭 가능한 모든 숫자를 시도한다.
            for (char c: kMapping[phone_number[digit] - '0']) {
                (*partial_mnemonic)[digit] = c;
                PhoneMnemonicHelper(phone_number, digit + 1,
                                    partial_mnemonic, mnemonics);
            }
        }

    }
}

// 3. n개의 퀸이 서로 공격하지 못하는 상황을 모두 나열하기
namespace problem_15_3 {
    using namespace std;

    void SolveQueens(int n, int row, vector<int> *col_placement, vector<vector<int>> *result);

    bool IsValid(const vector<int> &col_placement);

    vector<vector<int>> NQueens(int n) {
        vector<vector<int>> result;
        SolveQueens(n, 0, make_unique<vector<int>>().get(), &result);
        return result;
    }

    void SolveQueens(int n, int row, vector<int> *col_placement, vector<vector<int>> *result) {
        if (row == n) {
            // 모든 퀸을 놓을 수 있다.
            result->emplace_back(*col_placement);
        } else {
            for (int col = 0; col < n; ++col) {
                col_placement->emplace_back(col);
                if (IsValid(*col_placement)) {
                    SolveQueens(n, row + 1, col_placement, result);
                }
                col_placement->pop_back();
            }
        }
    }

    // 새로운 위치에 놓인 퀸이 기존에 있던 다른 퀸들에게 잡아먹히는 상황이 나오는지 확인한다.
    bool IsValid(const vector<int> &col_placement) {
        int row_id = size(col_placement) - 1;
        for (int i = 0; i < row_id; ++i) {
            if (int diff = abs(col_placement[i] - col_placement[row_id]); diff == 0 || diff == row_id - i) {
                // 열 또는 대각선 제약 조건을 위반
                return false;
            }

        }
        return true;
    }

}
//
// Created by jewoo on 2021-11-19.
//
#include <iostream>
#include <stdexcept>

using namespace std;

unsigned long long int Add(unsigned long long int a, unsigned long long int b);

short CountBits(unsigned int x) {
    short num_bits{0};
    while (x) {
        num_bits += x & 1;
        x >>= 1;
    }
    return num_bits;
}

// problem 4.1 패리티 계산하기
short Parity(unsigned long long x) {
    short result{0};
    while (x) {
        result ^= (x & 1);
        x >>= 1;
    }
    return result;
}

// 더 나은 버전
// x & (x - 1) = (00101100) & (00101011) = (00101000)
short Parity2(unsigned long long x) {
    short result{0};
    while (x) {
        result ^= 1;
        x &= (x - 1); // 하위 비트를 지운다
    }
    return result;
}

short Parity3(unsigned long long x) {
    x ^= x >> 32;
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;
    return x & 0x1;
}

// 4.2 비트 스왑
long long SwapBits(long long x, int i, int j) {
    if (((x >> i) & 1) != ((x >> j) & 1)) {
        unsigned long long bit_mask = (1L << i) | (1L << j);
        x ^= bit_mask;
    }
    return x;
}

// 4.4 같은 무게를 가진 가장 가까운 정수 찾기

unsigned long long ClosestIntSameBitCount(unsigned long long x) {
    const static int kNumUnsignedBits{64};
    for (int i = 0; i < kNumUnsignedBits - 1; ++i) {
        if (((x >> i) & 1) != ((x >> (i + 1)) & 1)) {
            x ^= (1UL << i) | (1UL << (i + 1));
            return x;
        }
    }
    throw std::invalid_argument("All bits are 0 or 1");
}

// 4.5 곱셈과 덧셈 없이  x X y 계산하기
unsigned long long Multiply(unsigned long long x,
                            unsigned long long y) {
    unsigned long long sum{0};
    while (x) {
        if (x & 1) {
            sum = Add(sum, y);
        }
        x >>= 1, y <<= 1;
    }
    return sum;
}

unsigned long long int Add(unsigned long long int a,
                           unsigned long long int b) {
    while (b) {
        unsigned long long carry = a & b;
        a = a ^ b;
        b = carry << 1;
    }
    return a;
}

// 4.6 산술 연산자 없이 나눗셈 계산하기
int Divide(int x, int y) {
    int result{0};
    int power{32};

    unsigned long long y_power = static_cast<unsigned long long>(y) << power;
    while (x >= y) {
        while (y_power > x) {
            y_power >>= 1;
            --power;
        }
        result += 1 << power;
        x -= y_power;
    }
    return result;
}

// 4.7 pow(x, y) 계산하기
double Power(double x, int y) {
    double result{1.0};
    long long power = y;
    if (y < 0) {
        power = -power, x = 1.0 / x;
    }
    while (power) {
        if (power & 1) {
            result *= x;
        }
        x *= x, power >>= 1;
    }
    return result;
}

// 4.8 숫자 뒤집기
long long Reverse(int x) {
    long long result{0};
    while (x) {
        result = result * 10 + x % 10;
        x /= 10;
    }
    return result;
}

// 4.9 회문 확인하기

bool IsPalindromeNumber(int x) {
    if (x <= 0) {
        return x == 0;
    }

    const int num_digits = static_cast<int> (floor(log10(x)) + 1);
    int msd_mask = static_cast<int>(pow(10, num_digits - 1));
    for (int i = 0; i < (num_digits / 2); ++i) {
        if (x / msd_mask != x % 10) {
            return false;
        }
        x %= msd_mask;
        x /= 10;
        msd_mask /= 100;
    }
    return true;
}

// 4.10 임의의 숫자를 균등한 확률로 생성하기
int UniformRandom(int lower_bound, int upper_bound) {
    int number_of_outcomes = upper_bound - lower_bound + 1, result;
    do {
        result = 0;
        for (int i = 0; (1 << i) < number_of_outcomes; ++i) {
            // ZeroOneRandom은 임의의 숫자를 생성하는 함수
            result = (result << 1) | ZeroOneRandom()
        }
    } while (result >= number_of_outcomes);
    return result + lower_bound;
}

// 4.11 사각형이 겹치는지 확인하기
struct Rect {
    int x, y, width, height;
};

bool IsIntersect(const Rect &r1, const Rect &r2) {
    return r1.x <= r2.x + r2.width && r1.x + r1.width >= r2.x &&
           r1.y <= r2.y + r2.height && r1.y + r1.height >= r2.y;
}

Rect IntersectRectangle(const Rect &r1, const Rect &r2) {
    if (!IsIntersect(r1, r2)) {
        return {0, 0, -1, -1};
    }
    return {max(r1.x, r2.x), max(r1.y, r2.y),
            min(r1.x + r1.width, r2.x + r2.width) - max(r1.x, r2.x),
            min(r1.y + r1.height, r2.y + r2.height) - max(r1.y, r2.y)};
}

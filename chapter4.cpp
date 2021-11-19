//
// Created by jewoo on 2021-11-19.
//

short CountBits(unsigned int x) {
    short num_bits{0};
    while (x) {
        num_bits += x & 1;
        x >>= 1;
    }
    return num_bits;
}

// problem 4.1 패리티 계산하기

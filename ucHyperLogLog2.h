// based on https://raw.githubusercontent.com/hideo55/cpp-HyperLogLog/master/include/hyperloglog.hpp

#pragma once

#include "uniquecounter.h"
#include "universalHashing.h"

#include <algorithm>
#include <cmath>

#define _GET_CLZ(x, b) (uint8_t)std::min(b, ::__builtin_clz(x)) + 1

class UniqCounterHyperLogLog2 {
    uint8_t b_; // register bit width
    uint32_t m_; // register size
    double alphaMM_; // alpha * m^2
    std::vector<uint8_t> M_; // registers
    HasherTrivial hasher;

public:
    UniqCounterHyperLogLog2(uint8_t b = 15) : b_(b), m_(1 << b), M_(m_, 0) {
        if (b < 4 || 30 < b) {
            throw std::invalid_argument("bit width must be in the range [4,30]");
        }
        double alpha = 0.7213 / (1.0 + 1.079 / m_);
        alphaMM_ = alpha * m_ * m_;
    }

    void add(int x) {
        uint32_t hash = hasher(x);
        uint32_t index = hash >> (32 - b_);
        uint8_t rank = _GET_CLZ((hash << b_), 32 - b_);
        if (rank > M_[index]) {
            M_[index] = rank;
        }
    }
    int get_uniq_num() const {
        double sum = 0.0;
        for (uint32_t i = 0; i < m_; i++) {
            sum += 1.0 / (1 << M_[i]);
        }
        double estimate = alphaMM_ / sum; // E in the original paper
        if (estimate <= 2.5 * m_) {
            uint32_t zeros = std::count(M_.begin(), M_.end(), 0);
            if (zeros != 0) {
                estimate = m_ * log((double)m_ / zeros);
            }
        } else if (estimate > (1 / 30.0) * (1ULL << 32)) {
            estimate = -(1ULL << 32) * log(1.0 - (estimate / (1ULL << 32)));
        }
        return estimate;
    }
    const char* getName() const {
        return "HyperLogLog by Hideaki Ohno";
    }
};

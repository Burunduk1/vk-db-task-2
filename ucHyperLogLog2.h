// based on 
//		https://raw.githubusercontent.com/hideo55/cpp-HyperLogLog/master/include/hyperloglog.hpp
//
// can not stably give even 10% relative error for all B in [5..15]

/** Experiment's results

b = 2^{15}
	test solution 'HyperLogLog by Hideaki Ohno' with N = 1000000, 6 ks, 16 tests
	................ maxTime = 0.00533041s, maxError = 0.85632, aveError = 0.85613
	test error for solution 'HyperLogLog by Hideaki Ohno' with N = 1000000, 7 ks, 14 tests
	..............  k =	 1000: [0.0150, 0.0150]
	..............  k =   100000: [0.8563, 0.8562]
	..............  k =   500000: [0.3301, 0.3212]
	..............  k =  1000000: [0.3989, 0.3845]
	..............  k =  2000000: [0.2006, 0.1932]
	..............  k = 10000000: [0.0548, 0.0471]
	..............  k = 30000000: [0.0247, 0.0150]

b = 2^{8}
	test solution 'HyperLogLog by Hideaki Ohno' with N = 1000000, 6 ks, 16 tests
	................ maxTime = 0.00224905s, maxError = 0.78861, aveError = 0.78853
	test error for solution 'HyperLogLog by Hideaki Ohno' with N = 1000000, 7 ks, 14 tests
	..............  k =	 1000: [0.2500, 0.2500]
	..............  k =   100000: [0.7886, 0.7885]
	..............  k =   500000: [0.4023, 0.3076]
	..............  k =  1000000: [0.5151, 0.3688]
	..............  k =  2000000: [0.2527, 0.1454]
	..............  k = 10000000: [0.2146, 0.0676]
	..............  k = 30000000: [0.1489, 0.0475]

Conclusion: 
	~15 times faster than ucFirstApproach.h
	for big k has not better error
	for small k does not work properly
*/

#pragma once

#include "uniquecounter.h"
#include "universalHashing.h"

#include <algorithm>
#include <cmath>

#define _GET_CLZ(x, b) (uint8_t)std::min(b, ::__builtin_clz(x)) + 1

// uses 2^B bytes
template<const int B> 
class UniqCounterHyperLogLog2 {
	uint8_t b; // register bit width
	uint32_t m; // register size
	double alphaMM; // alpha * m^2
	std::vector<uint8_t> M; // registers
	HasherTrivial hasher;

public:
	UniqCounterHyperLogLog2() : b(B), m(1 << b), M(m, 0) {
		if (b < 4 || 30 < b) {
			throw "UniqCounterHyperLogLog2: bit width must be in the range [4,30]";
		}
		double alpha = 0.7213 / (1.0 + 1.079 / m);
		alphaMM = alpha * m * m;
	}

	void add(int x) {
		uint32_t hash = hasher(x);
		uint32_t index = hash >> (32 - b);
		uint8_t rank = _GET_CLZ((hash << b), 32 - b);
		if (rank > M[index]) {
			M[index] = rank;
		}
	}
	int get_uniq_num() const {
		double sum = 0.0;
		for (uint32_t i = 0; i < m; i++) {
			sum += 1.0 / (1 << M[i]);
		}
		double estimate = alphaMM / sum; // E in the original paper
		if (estimate <= 2.5 * m) {
			uint32_t zeros = std::count(M.begin(), M.end(), 0);
			if (zeros != 0) {
				estimate = m * log((double)m / zeros);
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

// based on 
//		P. Flajolet, Éric Fusy, O. Gandouet, and F. Meunier. 
// 		Hyperloglog: The analysis of a near-optimal cardinality estimation algorithm. 
// 		In Analysis of Algorithms (AOFA), pages 127–146, 2007.
// idea
//		HyperLogLog for 2^B cells + store $smallN$ elements
//
// can not stably give even 10% relative error for all B in [5..14]

/** Experiment's results for a_i in [1..k]

B = 8, M = 256, smallN = 8096
	test solution 'hyperLogLog' with N = 100000, 6 ks, 16 tests
	................ maxTime = 0.00668794s, maxError = 0.93060, aveError = 0.68863
	test error for solution 'hyperLogLog' with N = 100000, 7 ks, 14 tests
	..............	k =     1000: [0.0000, 0.0000]
	..............	k =    10000: [0.9502, 0.5592]
	..............	k =    50000: [0.9396, 0.4413]
	..............	k =   100000: [0.5798, 0.2929]
	..............	k =   200000: [0.5217, 0.1957]
	..............	k =  1000000: [0.0973, 0.0469]
	..............	k =  3000000: [0.0575, 0.0290]

B = 14, M = 16384, smallN = 4064
	test solution 'hyperLogLog' with N = 100000, 6 ks, 16 tests
	................ maxTime = 0.00665117s, maxError = 4.82527, aveError = 4.81623
	test error for solution 'hyperLogLog' with N = 100000, 7 ks, 14 tests
	..............	k =     1000: [0.0000, 0.0000]
	..............	k =    10000: [4.8255, 4.8164]
	..............	k =    50000: [0.7438, 0.5338]
	..............	k =   100000: [0.4893, 0.3493]
	..............	k =   200000: [0.3596, 0.2203]
	..............	k =  1000000: [0.5825, 0.0977]
	..............	k =  3000000: [0.1512, 0.0422]
*/

#pragma once

#include "uniquecounter.h"
#include "universalHashing.h"

#include <algorithm>
#include <cstring>
#include <cmath>

// uses 2^B bytes for HyperLogLog itself
// uses 32K at all
template<const int B> 
class UniqCounterHyperLogLog : public UniqCounterInterface {
	static const int BYTES = (1 << 15) - 128; // additional O(1) <= 128 bytes
	static const int M = 1 << B; // cells to store sketches
	static const int smallN = (BYTES - M) / 4; // while cardinality of set < smallN, store all the set

	uint8_t bytes[BYTES]; 
	uint8_t *m; // m[M]
	int32_t n, *data; // data[smallN]
	Hasher hasherToM;

	static Hasher hasherBigNumber;
	static bool initialized;

	void addHyperLogLog(uint32_t x) {
		int i = hasherToM(x);
		x = hasherBigNumber(x);
		m[i] = std::max(m[i], (uint8_t)(__builtin_ctz(x) + 1));
	}

public:
	UniqCounterHyperLogLog() : m(bytes), n(0), data((int *)(m + M)), hasherToM(M) {
		if (!initialized) {
			initialized = true;
			fprintf(stderr, "B = %d, M = %d, smallN = %d\n", B, M, smallN);
			if (!(4 <= B && B <= 14))
				throw "B should be in [4..14]";
		}
		hasherBigNumber.rehash();
	}
	void add(int x) {
		if (n < smallN) {
			int i = std::lower_bound(data, data + n, x) - data;
			if (i < n && data[i] == x)
				return;
			memmove(data + i + 1, data + i, sizeof(data[0]) * (n - i));
			data[i] = x, n++;
			if (n == smallN) {
				std::fill(m, m + M, 0);
				for (int i = 0; i < n; i++)
					addHyperLogLog(data[i]);
			}
		} else {
			addHyperLogLog(x);
		}
	}
	int get_uniq_num() const {
		if (n < smallN)
			return n;
		const int BITS = 32;
		std::vector<int> cnt(1 + BITS);
		for (int i = 0; i < M; i++)
			if (m[i] > 0)
				cnt.at(m[i])++;
		double sum = 0, power = 1;
		for (int i = 1; i <= BITS; i++) {
			power *= 0.5;
			sum += power * cnt[i];
		}
		static const double alpha = 0.7213 / (1 + 1.079 / M); // formula from open sources
		// static const double alpha = 0.72129999569229; // (16384 * int x=0..\infty (log_2(2 + x) - log_2(1 + x))^{16384} dx)^{-1}
		// fprintf(stderr, "sum = %.10f, alpha = %.6f\n", sum, alpha);
		return alpha * M * M / sum;
	}
	const char* getName() const {
		return "hyperLogLog";
	}
};

template<const int B> bool UniqCounterHyperLogLog<B>::initialized = false;
template<const int B> Hasher UniqCounterHyperLogLog<B>::hasherBigNumber(1 << 30);

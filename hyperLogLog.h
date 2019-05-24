// P. Flajolet, Éric Fusy, O. Gandouet, and F. Meunier. 
// Hyperloglog: The analysis of a near-optimal cardinality estimation algorithm. 
// In Analysis of Algorithms (AOFA), pages 127–146, 2007.

// uses sketch M 8-bit sketches, uses all other 32*1024-M bytes to store small answers
// can not stably give even 10% relative error for all M = 2^{5..14} 

#pragma once

#include "uniquecounter.h"
#include "universalHashing.h"

#include <algorithm>
#include <cstring>
#include <cmath>

template<const int BYTES_>
class UniqCounterHyperLogLog : public UniqCounterInterface {
	static const int BYTES = BYTES_ - 128; // additional O(1) <= 128 bytes
	static const int M = 1 << 13; // cells to store sketches
	static const int smallN = (BYTES - M) / 4; // while cardinality of set < smallN, store all the set

	uint8_t bytes[BYTES]; 
	uint8_t *m; // m[M]
	int32_t n, *data; // data[smallN]
	Hasher hasherToM;
	Hasher hasherBigNumber;

	void addHyperLogLog(uint32_t x) {
		int i = hasherToM(x);
		x = hasherBigNumber(x);
		m[i] = std::max(m[i], (uint8_t)(__builtin_ctz(x) + 1));
	}

public:
	UniqCounterHyperLogLog() : m(bytes), n(0), data((int *)(m + M)), hasherToM(M), hasherBigNumber(1 << 30) {
		fprintf(stderr, "M = %d, smallN = %d\n", M, smallN);
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
		printf("sum = %.10f, alpha = %.6f\n", sum, alpha);
		return alpha * M * M / sum;
	}
	const char* getName() const {
		return "hyperLogLog";
	}
};

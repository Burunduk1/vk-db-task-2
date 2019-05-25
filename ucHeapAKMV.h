// idea: 
//		generate x_1...x_n uniformly in 1..M
//		k-th ordered statistic approximation = M*k/(n+1) 
//		appoximation of n = M/(k-th statistic)-1
// data structure:
//		heap<uint32_t>

#pragma once

#include "uniquecounter.h"
#include "universalHashing.h"

#include <algorithm>
#include <cmath>

template<const int BYTES>
class UniqCounterAKMV : public UniqCounterInterface {
	static const int N = BYTES / sizeof(int) - 1; 
	int n;
	uint32_t a[BYTES];

	static HasherP hasherBigNumber; 
	static bool initialized;

public:
	UniqCounterAKMV() : n(0) {	
		hasherBigNumber.rehash();
		if (!initialized) {
			initialized = true;
			fprintf(stderr, "\nUniqCounterAKMV(%d) : store %d elements\n", BYTES, N);
		}
	} 

	void add(int x) { // O(n)
		assert(n <= N - 1);
		uint32_t hashValue = hasherBigNumber(x);
		if (std::find(a, a + n, hashValue) != a + n)
			return;
		a[n++] = hashValue;
		if (n == N - 1) {
			std::make_heap(a, a + n);
		}
		if (n == N) {
			std::push_heap(a, a + n);
			std::pop_heap(a, a + n);
			n--;
		}
	}
	int get_uniq_num() const { // O(nlogn)
		uint32_t *b = (uint32_t *)a; // interface says "const!" but this approach would like to modify a[] to minimize memory overhead.
		assert(n <= N - 1);
		if (n < N - 1)
			return n; // exact small answers
		std::sort_heap(b, b + n);
		auto statistic = [&](int k) {
			return round((double)hasherBigNumber.range() / b[k - 1] * k) - 1;
		};
		auto result = statistic(n);
		result = result < 3 * N ? statistic(n / 2) : result;
		std::make_heap(b, b + n);
		return result;
	}
	const char* getName() const {
		return "AKMV via heap";
	}
};

template<const int BYTES> bool UniqCounterAKMV<BYTES>::initialized = false;
template<const int BYTES> HasherP UniqCounterAKMV<BYTES>::hasherBigNumber(Primes::next(-110)); 

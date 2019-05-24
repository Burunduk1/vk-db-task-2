// idea: 
//		generate x_1...x_n uniformly in 1..M
//		k-th ordered statistic approximation = M*k/(n+1) 
//		appoximation of n = M/(k-th statistic)-1

#pragma once

#include "uniquecounter.h"
#include "universalHashing.h"

#include <set>
#include <cmath>

template<const int BYTES>
class UniqCounterFirstApproach : public UniqCounterInterface {
	static const int N = BYTES / sizeof(std::set<int>::node_type); 
	static HasherP hasherBigNumber; 
	std::set<unsigned> s;
	int n;
	
	static bool initialized;

public:
	UniqCounterFirstApproach() : n(0) {	
		hasherBigNumber.rehash();
		if (!initialized) {
			initialized = true;
			fprintf(stderr, "\nUniqCounterFirstApproach(%d) : %ld bytes per set::node => store %d elements\n", BYTES, sizeof(std::set<int>::node_type), N);
		}
	} 

	void add(int x) {
		uint32_t hashValue = hasherBigNumber(x);
		// uint32_t hashValue = HasherTrivial::get(x);
		// uint32_t hashValue = (uint32_t)(1e9 + 7) * (uint32_t)x + (uint32_t)3e8;
		s.insert(hashValue);
		if (s.size() > N)
			s.erase(--s.end());
		n++;
	}
	int get_uniq_num() const {
		if (s.size() < N) 
			return s.size(); // exact small answers
		auto result = round((double)(1ULL << 32) / *s.rbegin() * N) - 1; // use N-th ordered statistic 
		if (result < 3 * N) {
			// fprintf(stderr, "extremal case: %g < 3 * %d\n", result, N);
			int k = N / 2;
			auto it = s.begin();
			for (int i = 0; i < k - 1; i++)
				++it;
			return round((double)(1ULL << 32) / *it * k) - 1; // use (N/2)-th ordered statistic 
		}
		return result;
	}
	const char* getName() const {
		return "firstApproach";
	}
};

template<const int BYTES> bool UniqCounterFirstApproach<BYTES>::initialized = false;
template<const int BYTES> HasherP UniqCounterFirstApproach<BYTES>::hasherBigNumber(Primes::next(-110)); 

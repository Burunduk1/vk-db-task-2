// idea: 
//		generate x_1...x_n uniformly in 1..M
//		k-th ordered statistic approximation = M*k/(n+1) 
//		appoximation of n = M/(k-th statistic)-1
// data structure:
//		set<uint32_t>

#pragma once

#include "uniquecounter.h"
#include "universalHashing.h"

#include <set>
#include <cmath>
#include <utility>

template<const int BYTES>
class UniqCounterFirstApproach : public UniqCounterInterface {
	static const int N = BYTES / sizeof(std::set<int>::node_type); 
	static HasherP hasher32; 
	// Hasher32 hasher32; 
	// HasherTrivial hasher32; 
	// std::hash<int> hasher32;
	std::set<uint32_t> s;
	int n;
	
	static bool initialized;

public:
	UniqCounterFirstApproach() : n(0) {	
		if (!initialized) {
			initialized = true;
			fprintf(stderr, "\nUniqCounterFirstApproach(%d) : %ld bytes per set::node => store %d elements\n", BYTES, sizeof(std::set<int>::node_type), N);
		}
		hasher32.rehash();
	} 

	void add(int x) {
		uint32_t hashValue = hasher32(x);
		s.insert(hashValue);
		if (s.size() > N)
			s.erase(--s.end());
		n++;
	}
	int get_uniq_num() const {
		if (s.size() < N) 
			return s.size(); // exact small answers
		auto statistic = [&](int k, uint32_t x) {
			return round((double)hasher32.range() / x * k) - 1; // (1ULL << 32)
		};
		auto result = statistic(N, *s.rbegin());
		if (result < 3 * N) {
			int k = N / 2;
			auto it = s.begin();
			for (int i = 0; i < k - 1; i++)
				++it;
			return statistic(k, *it);
		}
		return result;
	}
	const char* getName() const {
		return "firstApproach";
	}
};

template<const int BYTES> bool UniqCounterFirstApproach<BYTES>::initialized = false;
template<const int BYTES> HasherP UniqCounterFirstApproach<BYTES>::hasher32(Primes::next(-80)); 

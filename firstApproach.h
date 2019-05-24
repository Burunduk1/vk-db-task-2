#pragma once

#include "uniquecounter.h"

#include <set>

class UniqCounterFirstApproach : public UniqCounterInterface {
    static const size_t N = 500; // assume 64 bytes per node
    static const unsigned A = 1e9 + 7, B = 3e8; // hash function: x --> x*A+B mod 2^{32}
	std::set<unsigned> s;
    int n;
public:    
    UniqCounterFirstApproach() : n(0) {
    }
	void add(int x) {
        unsigned hashValue = (unsigned)x * A + B;
		s.insert(hashValue);
        if (s.size() > N)
            s.erase(--s.end());
        n++;
    }
    int get_uniq_num() const {
        if (s.size() < N) 
            return s.size(); // exact small answers
    	return ((double)(1ULL << 32) / *s.rbegin() * N + 0.5) - 1; // use N-th ordered statistic
    }
    const char* getName() const {
    	return "trivial";
    }
};

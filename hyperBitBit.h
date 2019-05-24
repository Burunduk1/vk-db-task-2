// https://www.cs.princeton.edu/~rs/talks/Cardinality.pdf
// uses 144 bits (64+64+16), gives ~10-20% error with p ~= 0.5 for big N

#pragma once

#include "uniquecounter.h"
#include "universalHashing.h"

#include <algorithm>
#include <cmath>

static const int BITS = 30;

//static Hasher hasherTo64(64);
static Hasher hasherToBigNumber(1 << BITS);

class UniqCounterHyperBitBit : public UniqCounterInterface {
	uint16_t logN;
	uint64_t s1, s2;

public:
	UniqCounterHyperBitBit() : logN(5), s1(0), s2(0) { }

	void add(int x) {
		auto hash = hasherToBigNumber(x);
		auto r = __builtin_ctz(hash);
		if(r > logN) {
			const int k = hash >> (BITS - 6); // 0..63
			s1 |= 1LL << k;
			if (r > logN + 1) {
				s2 |= 1LL << k;
			}
			if (__builtin_popcountll(s1) > 31) {
				s1 = s2, s2 = 0, logN++;
			}
		}
	}

	int get_uniq_num() const {
		return pow(2., (logN + 5.4 + __builtin_popcountll(s1) / 32.)); // or 5.15?
	}

	const char* getName() const {
		return "hyperBitBit";
	}
};

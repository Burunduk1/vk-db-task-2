#include "test.h"

#include "ucTrivialSolution.h"
#include "ucFirstApproach.h"
#include "ucHyperLogLog.h"
#include "ucHyperBitBit.h"

#include <set>

int main() {
	try {
		const int N = 1 << 15;
		// test<UniqCounterTrivial>();
		test<UniqCounterFirstApproach<1 << 9>>();
		test<UniqCounterFirstApproach<1 << 12>>();
		test<UniqCounterFirstApproach<N>>();
		test<UniqCounterFirstApproach<1 << 17>>();
		// test<UniqCounterHyperLogLog<N>>();
		// test<UniqCounterHyperBitBit>();
	} catch (const char* msg) {
		fprintf(stderr, "catched error: %s\n", msg);
	}
}

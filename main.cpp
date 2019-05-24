#include "test.h"

#include "trivialSolution.h"
#include "firstApproach.h"
#include "hyperLogLog.h"
#include "hyperBitBit.h"

#include <set>

int main() {
	// test<UniqCounterTrivial>();
	try {
		const int N = 32 << 10;
		// test<UniqCounterFirstApproach<N>>();
		// test<UniqCounterHyperLogLog<N>>();
		test<UniqCounterHyperBitBit>();
	} catch (const char* msg) {
		fprintf(stderr, "catched error: %s\n", msg);
	}
}

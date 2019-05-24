#include "test.h"

#include "ucTrivialSolution.h"
#include "ucFirstApproach.h"
#include "ucHyperLogLog.h"
#include "ucHyperBitBit.h"

#include <set>

int main() {
	try {
		const int N = 32 << 10;
		// test<UniqCounterTrivial>();
		test<UniqCounterFirstApproach<N>>();
		// test<UniqCounterHyperLogLog<N>>();
		// test<UniqCounterHyperBitBit>();
	} catch (const char* msg) {
		fprintf(stderr, "catched error: %s\n", msg);
	}
}

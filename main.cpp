#include "test.h"

#include "hyperLogLog.h"
#include "trivialSolution.h"
#include "firstApproach.h"

#include <set>

int main() {
	// test<UniqCounterTrivial>();
	try {
		const int N = 32 << 10;
		test<UniqCounterFirstApproach<N>>();
		// test<UniqCounterHyperLogLog<N>>();
	} catch (const char* msg) {
		fprintf(stderr, "catched error: %s\n", msg);
	}
}

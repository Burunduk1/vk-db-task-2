#include "test.h"

#include "ucTrivialSolution.h"
#include "ucHyperLogLog.h"
#include "ucHyperLogLog2.h"
#include "ucHyperBitBit.h"
#include "ucFirstApproach.h"
#include "ucHeapAKMV.h"

#include <set>

int main() {
	try {
		const int BYTES = 1 << 15, N = 1e5, iters = 1e7;
		// test<UniqCounterTrivial>();
		// test<UniqCounterFirstApproach<1 << 9>>();
		// test<UniqCounterFirstApproach<1 << 12>>();
		// test<UniqCounterFirstApproach<N>>();
		// test<UniqCounterFirstApproach<1 << 17>>();
		// test<UniqCounterHyperLogLog<BYTES>>();
		// test<UniqCounterHyperBitBit>();
		// analyse<UniqCounterAKMV<BYTES>>(N, iters);
		// analyseError<UniqCounterAKMV<BYTES>>(N, iters);
		// analyse<UniqCounterFirstApproach<BYTES>>(N, iters);
		// analyseError<UniqCounterFirstApproach<BYTES>>(N, iters);
		
		analyse<UniqCounterHyperLogLog<8>>(N, iters);
		analyseError<UniqCounterHyperLogLog<8>>(N, iters);

		analyse<UniqCounterHyperLogLog<14>>(N, iters);
		analyseError<UniqCounterHyperLogLog<14>>(N, iters);
	} catch (const char* msg) {
		fprintf(stderr, "catched error: %s\n", msg);
	}
}

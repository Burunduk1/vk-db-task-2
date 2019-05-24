#include "test.h"

#include "hyperLogLog.h"
#include "trivialSolution.h"
#include "firstApproach.h"

#include <set>

int main() {
	// printf("%ld\n", sizeof(std::set<int>::node_type));
	// return 0;
	// test<UniqCounterTrivial>();
	test<UniqCounterFirstApproach>();
	test<UniqCounterHyperLogLog>();
}
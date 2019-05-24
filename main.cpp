#include "test.h"

#include "hyperLogLog.h"
#include "trivialSolution.h"

int main() {
	test<UniqCounterTrivial>();
	test<UniqCounterHyperLogLog>();
}
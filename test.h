#pragma once

#include "uniquecounter.h"
#include "timer.h"
#include "randomNumbers.h"

#include <unordered_set>
#include <vector>
#include <random>
#include <cassert>

template<typename SolutionType> void test() {
	auto relative_error = [&](int expected, int got) {
		return abs(got - expected) / (double) expected;
	};

	const int N = (int)3e6;
	printf("test solution '%s' with N = %d\n", SolutionType().getName(), N);
	std::vector<int> test(N);
	std::mt19937 gen = newGen();
	for (int k : {10, 1000, 10000, N / 10, N, N * 10, N * 100}) {
		std::uniform_int_distribution<> dis(1, k);
		for (int &x : test) {
			x = dis(gen);
		}

		TimerNano t;
		SolutionType counter;
		for (int i = 0; i < N; i++) {
			counter.add(test[i]);
		}
		int counter_result = counter.get_uniq_num();
		TimeType nanos = t.time();
		printf("%d numbers in range [1..%d], %gs, %d result, ", N, k, nanos * 1e-9, counter_result);

		int expected = std::unordered_set<int>(test.begin(), test.end()).size();
		double error = relative_error(expected, counter_result);
		printf("%d uniq, %g relative error\n", expected, error);
		// assert(error <= 0.1);
	}
}

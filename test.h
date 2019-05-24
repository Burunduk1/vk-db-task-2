#pragma once

#include "uniquecounter.h"
#include "timer.h"

#include <set>
#include <random>
#include <cassert>

template<typename SolutionType> void test() {
	std::random_device rd;
	std::mt19937 gen(rd());

	auto relative_error = [&](int expected, int got) {
		return abs(got - expected) / (double) expected;
	};

	const int N = (int)1e6;
	printf("test solution '%s' with N = %d\n", SolutionType().getName(), N);
	for (int k : {1, 10, 1000, 10000, N / 10, N, N * 10}) {
		std::uniform_int_distribution<> dis(1, k);
		std::set<int> all;
		SolutionType counter;
		TimerNano t;
		for (int i = 0; i < N; i++) {
			int value = dis(gen);
			all.insert(value);
			counter.add(value);
		}
		TimeType nanos = t.time();
		int expected = (int) all.size();
		int counter_result = counter.get_uniq_num();
		double error = relative_error(expected, counter_result);
		printf("%d numbers in range [1..%d], %gns, %d uniq, %d result, %g relative error\n", 
			N, k, nanos * 1e-9, expected, counter_result, error);
		assert(error <= 0.1);
	}
}

#pragma once

#include "uniquecounter.h"
#include "randomNumbers.h"
#include "timer.h"

#include <unordered_set>
// #include <algorithm>
#include <vector>
#include <random>
#include <cassert>

auto relativeError(int expected, int got) {
	return abs(got - expected) / (double) expected;
}

auto offlineSolution(std::vector<int> &test) {
	// std::sort(test.begin(), test.end());
	// return std::unique(test.begin(), test.end()) - test.begin();
	return std::unordered_set<int>(test.begin(), test.end()).size();
}

// n uniformly distributed numbers in [1..k]
auto genTest(int N, int k) {
	static std::mt19937 gen = newGen();
	std::uniform_int_distribution<> dis(1, k);
	std::vector<int> test(N);
	for (int &x : test) {
		x = dis(gen);
	}
	return test;
}

template<typename SolutionType>
auto run(const std::vector<int> &test) {
	// printf("run(%lu)", test.size()), fflush(stdout);
	TimerNano t;
	SolutionType counter;
	for (auto x : test) {
		counter.add(x);
	}
	int counter_result = counter.get_uniq_num();
	return std::make_pair(counter_result, t.time());
}

#define ERROR_ASSERT \
	if (error > 0.3) { \
		fprintf(stderr, "N = %d, k = %d, expected = %d, error = %g\n", N, k, expected, error); \
		throw "error > 0.3"; \
	}

template<typename SolutionType> void test() {
	const int N = 1e6;
	printf("test solution '%s' with N = %d\n", SolutionType().getName(), N);
	for (int k : {1, 10, 1000, 10000, N / 10, N / 2, N, 2 * N, N * 10}) {
	// for (int k : {10, 1000, 10000, N / 10, N, N * 10, N * 100}) {
		auto test = genTest(N, k);
		auto [counter_result, nanos] = run<SolutionType>(test);
		printf("%d numbers in range [1..%d], %gs, %d result, ", N, k, nanos * 1e-9, counter_result);

		int expected = offlineSolution(test);
		double error = relativeError(expected, counter_result);
		printf("%d uniq, %.6f relative error\n", expected, error);
		ERROR_ASSERT
	}
}

template<typename SolutionType> void analyse(int N, int64_t iterations = 1e8) {
	TimeType maxTime = 0;
	double maxError = 0, aveError = 0;
	std::vector<int> ks = {1000, N / 10, N / 2, N, 2 * N, N * 10};
	int testsN = iterations / N / ks.size(), cnt = 0;
	printf("test solution '%s' with N = %d, %ld ks, %d tests\n", SolutionType().getName(), N, ks.size(), testsN);
	for (int i = 0; i < testsN; i++) {
		printf("."), fflush(stdout);
		double error = 0;
		for (int k : ks) {
			auto test = genTest(N, k);
			auto [counter_result, nanos] = run<SolutionType>(test);
			maxTime = std::max(maxTime, nanos);

			int expected = offlineSolution(test);
			error = std::max(error, relativeError(expected, counter_result));
			ERROR_ASSERT
		}
		maxError = std::max(maxError, error);
		aveError += error, cnt++;
	}
	printf(" maxTime = %gs, maxError = %.5f, aveError = %.5f\n", maxTime * 1e-9, maxError, aveError / cnt);
}

template<typename SolutionType> void analyseError(int N, int64_t iterations = 1e8) {
	std::vector<int> ks = {1000, N / 10, N / 2, N, 2 * N, N * 10, N * 30};
	int testsN = iterations / N / ks.size();
	printf("test error for solution '%s' with N = %d, %ld ks, %d tests\n", SolutionType().getName(), N, ks.size(), testsN);
	for (int k : ks) {
		double maxError = 0, aveError = 0;
		int cnt = 0;
		for (int i = 0; i < testsN; i++) {
			printf("."), fflush(stdout);
			auto test = genTest(N, k);
			auto [counter_result, nanos] = run<SolutionType>(test);

			int expected = offlineSolution(test);
			double error = relativeError(expected, counter_result);
			maxError = std::max(maxError, error);
			aveError += error, cnt++;
			ERROR_ASSERT
		}
		printf("	k = %8d: [%.4f, %.4f]\n", k, maxError, aveError / cnt);
	}
}

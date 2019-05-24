#pragma once

#include "randomNumbers.h"

// unsigned --> 0..p-1, p should be prime
class HasherPrimeM {
	static std::mt19937 gen;
	uint32_t p, a, b;

public:
	HasherPrimeM(int p) : p(p) {
		do {
			a = gen() % p;
		} while (!a);
		b = gen() % p;
	}
	uint32_t operator() (uint32_t x) {
		return ((uint64_t)x * a + b) % p;
	}
};
std::mt19937 HasherPrimeM::gen = newGen();

// unsigned --> 0..m-1
class Hasher {
	int32_t m;
	HasherPrimeM hasher;

	bool isPrime(int x) {
		for (int i = 2; i * i <= x; i++)
			if (x % i == 0)
				return 0;
		return x >= 2;
	}
	int nextPrime(int p) {
		while (!isPrime(p))
			p++;
		return p;
	}
public:
	Hasher(int32_t m) : m(m), hasher(nextPrime(m)) {
	}
	int operator() (uint32_t x) {
		return hasher(x) % m;
	}
};

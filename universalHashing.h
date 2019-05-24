#pragma once

#include "randomNumbers.h"

// unsigned --> 0..p-1, p should be prime
class HasherPrimeM {
	unsigned p, a, b;

public:
	HasherPrimeM(int p) : p(p) {
		do
			a = gen() % p;
		while (!a);
		b = gen() % p;
	}
	int operator() (unsigned x) {
		return ((unsigned long long)x * a + b) % p;
	}
};

// unsigned --> 0..m-1
class Hasher {
	int m;
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
	Hasher(int m) : m(m), hasher(nextPrime(m)) {
	}
	int operator() (unsigned x) {
		return hasher(x) % m;
	}
};

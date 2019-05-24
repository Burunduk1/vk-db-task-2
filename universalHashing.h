#pragma once

#include "randomNumbers.h"

struct Primes {
	static bool is(uint32_t x) {
		for (uint32_t i = 2; i * i <= x; i++)
			if (x % i == 0)
				return 0;
		return x >= 2u;
	}
	static uint32_t next(uint32_t p) {
		while (!is(p))
			p++;
		return p;
	}
};

// uint32 --> 0..p-1, p should be prime
class HasherPrime {
	static std::mt19937 gen;
	uint32_t p, a, b;

public:
	HasherPrime(uint32_t p) : p(p) {
		do {
			a = gen() % p;
		} while (!a);
		b = gen() % p;
	}
	uint32_t operator() (uint32_t x) {
		return ((uint64_t)x * a + b) % p;
	}
};
std::mt19937 HasherPrime::gen = newGen();

// uint32 --> 0..m-1
class Hasher {
	int32_t m;
	HasherPrime hasher;

public:
	Hasher(int32_t m) : m(m), hasher(Primes::next(m)) {
	}
	uint32_t operator() (uint32_t x) {
		return hasher(x) % m;
	}
};

// uint32 --> 0..p-1, p >= m
class HasherP {
	HasherPrime hasher;

public:
	HasherP(int32_t m) : hasher(Primes::next(m)) {
	}
	uint32_t operator() (uint32_t x) {
		return hasher(x);
	}
};

// uint32 --> uint32
struct HasherTrivial {
	static const unsigned A = 1e9 + 7, B = 3e8;
	static uint32_t get(uint32_t x) {
		return A * x + B;
	}
};

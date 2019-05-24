#pragma once
    
#include "uniquecounter.h"

#include <set>

class UniqCounterTrivial : public UniqCounterInterface {
	std::set<int> s;
public:	
	void add(int x) {
		s.insert(x);
	}
	int get_uniq_num() const {
		return s.size();
	}
	const char* getName() const {
		return "trivial";
	}
};

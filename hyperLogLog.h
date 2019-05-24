#pragma once

#include "uniquecounter.h"

class UniqCounterHyperLogLog : public UniqCounterInterface {
public:    
	void add(int x) {

    }
    int get_uniq_num() const {
    }
    const char* getName() const {
    	return "hyperLogLog";
    }
};

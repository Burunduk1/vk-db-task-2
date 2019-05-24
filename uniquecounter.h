#pragma once

class UniqCounterInterface {
    // no more than 32kb of memory should be used here
public:
    virtual void add(int x) = 0;
    virtual int get_uniq_num() const = 0;
    virtual const char* getName() const = 0;
};

template<typename SolutionType> void test();

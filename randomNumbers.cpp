#include "randomNumbers.h"

std::mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());

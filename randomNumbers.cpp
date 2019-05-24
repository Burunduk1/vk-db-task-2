#include "randomNumbers.h"

std::mt19937 newGen() {
	return std::mt19937(std::chrono::system_clock::now().time_since_epoch().count());
}

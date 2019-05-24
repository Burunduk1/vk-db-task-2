#pragma once

#include <chrono>

typedef long long TimeType;

class TimerNano {
private:
	std::chrono::steady_clock::time_point startPoint;
public:
	TimerNano() {
		start();
	}
	void start() { 
		startPoint = std::chrono::steady_clock::now();
	}
	TimeType time() {
		auto endPoint = std::chrono::steady_clock::now();
		return std::chrono::duration_cast<std::chrono::nanoseconds>(endPoint - startPoint).count();
	}
};

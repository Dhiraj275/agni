#pragma once
#include <chrono>

class AgniTimer
{
public:
	AgniTimer();
	float Mark();
	float Peek() const;
private:
	std::chrono::steady_clock::time_point last;

};
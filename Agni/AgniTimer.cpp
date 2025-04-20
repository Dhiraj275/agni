#include "AgniTimer.h"

using namespace std::chrono;
AgniTimer::AgniTimer()
{
	last = steady_clock::now();
}

float AgniTimer::Mark()
{
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frameTime = last - old;
	return frameTime.count();
}

float AgniTimer::Peek() const
{
	return duration<float>(steady_clock::now() - last).count();
}

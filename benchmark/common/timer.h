#pragma once

#include <chrono>

/// A flexible timer. time_t is the precision of the timing, while scaling_factor
/// is the factor by which the output will be scaled. The default is to print
/// return milliseconds with microsecond precision.
template<typename time_t = std::chrono::microseconds, int scaling_factor = 1000, typename return_type = double>
struct FlexTimer { // for lack of a better name
	FlexTimer() {
		reset();
	}

	void reset() {
		start = std::chrono::system_clock::now();
	}

	return_type get() const {
		time_t duration = std::chrono::duration_cast<time_t>(std::chrono::system_clock::now() - start);
		return (duration.count() * 1.0) / scaling_factor;
	}

	return_type getAndReset() {
		auto t = get();
		reset();
		return t;
	}

private:
	std::chrono::system_clock::time_point start;
};

/// A timer that is accurate to microseconds, formatted as milliseconds
typedef FlexTimer<std::chrono::microseconds, 1000, double> MTimer;
/// A timer that is accurate to milliseconds, formatted as seconds
typedef FlexTimer<std::chrono::milliseconds, 1000, double> Timer;

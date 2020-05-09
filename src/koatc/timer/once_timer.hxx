#pragma once

#include <limits>

namespace turenar::koatc::timer {
class once_timer {
public:
	constexpr explicit once_timer(int interval) : _interval(interval) {}

	bool tick(int time) {
		if (!active()) {
			return false;
		}
		bool wake_up = time < _last_time - _interval || _last_time + _interval < time;
		if (wake_up) {
			_last_time = invalid_time;
		}
		return wake_up;
	}
	void activate(int time) {
		_last_time = time;
	}
	void inactivate() {
		_last_time = invalid_time;
	}
	[[nodiscard]] bool active() const {
		return _last_time != invalid_time;
	}

private:
	static constexpr int invalid_time = std::numeric_limits<int>::max();
	const int _interval;
	int _last_time = invalid_time;
};
} // namespace turenar::koatc::timer

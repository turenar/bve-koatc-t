#pragma once

namespace turenar::koatc::timer {
class continuous_timer {
public:
	constexpr explicit continuous_timer(int interval) : _interval(interval) {}

	bool tick(int time) {
		bool wake_up = time < _last_time - _interval || _last_time + _interval < time;
		if (wake_up) {
			_last_time = time;
		}
		return wake_up;
	}
	void reset(int time) {
		_last_time = time;
	}

private:
	const int _interval;
	int _last_time = 0;
};
} // namespace turenar::koatc

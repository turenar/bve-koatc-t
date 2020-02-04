#pragma once

namespace turenar::koatc {
class timer {
public:
	constexpr explicit timer(int interval) : _interval(interval) {}

	bool wake(int time) {
		bool wake_up = time < _last_time - _interval || _last_time + _interval < time;
		if (wake_up) {
			_last_time = time;
		}
		return wake_up;
	}
	bool waiting_once(int time) {
		return time <= _last_time + _interval;
	}
	void reset(int time) {
		_last_time = time;
	}

private:
	const int _interval;
	int _last_time = 0;
};
} // namespace turenar::koatc

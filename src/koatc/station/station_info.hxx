#pragma once

#include <spdlog/spdlog.h>

namespace turenar::koatc::station {
class station_info {
	constexpr static int no_pattern = 999;

public:
	constexpr station_info() = default;
	station_info(int number, bool stop) noexcept : _number(number), _stop(stop) {}

	constexpr explicit operator bool() const {
		return _number >= 0;
	}
	[[nodiscard]] constexpr int number() const {
		return _number;
	}
	[[nodiscard]] constexpr bool stop() const {
		return _stop;
	}
	[[nodiscard]] double location() const {
		return _location;
	}
	[[nodiscard]] int speed() const {
		return _speed;
	}
	[[nodiscard]] int bottom() const {
		return _bottom;
	}

	void control_stop(double location, int speed, int bottom) {
		if (_stop) {
			_location = location;
			_speed = speed;
			_bottom = bottom;
		}
	}

private:
	int _number = -1;
	bool _stop = false;
	double _location = 0;
	int _speed = no_pattern;
	int _bottom = no_pattern;
};
} // namespace turenar::koatc::station

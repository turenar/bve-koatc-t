#pragma once

#include <limits>
#include <spdlog/fmt/ostr.h>

namespace turenar::koatc::pattern {
class pattern_generator {
private:
	constexpr static double minimum_location = std::numeric_limits<int>::min();
	constexpr static double maximum_location = std::numeric_limits<int>::max();

public:
	constexpr static int no_pattern = 999;

	explicit pattern_generator(double deceleration);

	void use_emergency(bool flag) {
		_use_emergency = flag;
	}

	void update_location(double loc);
	void set_target_speed(double location, int speed);
	void clear();

	[[nodiscard]] double emergency_limit() const;
	[[nodiscard]] double normal_limit() const {
		return _current_limit;
	}
	[[nodiscard]] int bottom() const {
		return _current_bottom;
	}

private:
	/** m/h^2 */
	double _deceleration;

	double _zero_location = minimum_location;
	double _flat_start_location = maximum_location;
	int _curve_target = no_pattern;
	int _curve_bottom = no_pattern;
	int _flat_bottom = no_pattern;

	double _location = minimum_location;
	double _current_limit = no_pattern;
	int _current_bottom = no_pattern;

	bool _use_emergency = false;

	template <typename OStream>
	friend OStream& operator<<(OStream& os, const pattern_generator& c) {
		os << "current: " << c._current_limit << " -> " << c._current_bottom << ", curve: " << c._curve_target << '@';
		if (c._flat_start_location >= pattern_generator::maximum_location) {
			os << "null";
		} else {
			os << c._flat_start_location;
		}
		os << " -> " << c._curve_bottom << " -> 0@";
		if (c._zero_location <= pattern_generator::minimum_location) {
			os << "null";
		} else {
			os << c._zero_location;
		}
		os << ", flat: " << c._flat_bottom;
		return os;
	}
};
} // namespace turenar::koatc::pattern

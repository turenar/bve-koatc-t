#pragma once

#include <limits>
#include <spdlog/fmt/ostr.h>
#include "ats/vehicle_state.hxx"
#include "koatc/pattern/handle_command.hxx"

namespace turenar::koatc::pattern {
class pattern_generator {
protected:
	constexpr static double minimum_location = std::numeric_limits<int>::min();
	constexpr static double maximum_location = std::numeric_limits<int>::max();

	constexpr static double safety_pattern_offset = 15;
	constexpr static double deceleration = 2.;

public:
	constexpr static int no_pattern = 999;

	pattern_generator(const bve::ats::vehicle_state& state, double deceleration);

	void set_target_speed(double location, int speed);
	void clear();

	void tick();

	[[nodiscard]] double limit() const {
		return _current_limit;
	}
	[[nodiscard]] int bottom() const {
		return _current_bottom;
	}
	[[nodiscard]] handle_command handle() const {
		return _handle;
	}

protected:
	const bve::ats::vehicle_state& _vehicle_state;
	/** km/1000 h^2 */
	double _deceleration;

	double _zero_location = maximum_location;
	double _flat_start_location = minimum_location;
	int _curve_target = no_pattern;
	int _curve_bottom = no_pattern;
	int _flat_bottom = no_pattern;

	double _current_limit = no_pattern;
	int _current_bottom = no_pattern;
	handle_command _handle = handle_command::neutral();

	template <typename OStream>
	friend OStream& operator<<(OStream& os, const pattern_generator& c) {
		os << "brake: " << c._handle << ", current: " << c._current_limit << " -> " << c._current_bottom
		   << ", curve: " << c._curve_target << '@';
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

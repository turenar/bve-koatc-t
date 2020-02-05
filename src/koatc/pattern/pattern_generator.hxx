#pragma once

#include <limits>
#include <spdlog/fmt/ostr.h>
#include "ats/vehicle_state.hxx"
#include "koatc/pattern/curve_pattern.hxx"
#include "koatc/pattern/flat_pattern.hxx"
#include "koatc/pattern/handle_command.hxx"

namespace turenar::koatc::pattern {
class pattern_generator {
public:
	pattern_generator(const bve::ats::vehicle_state& state, double deceleration);

	void set_flat_speed(int speed);
	void set_target_speed(double location, int speed);
	void set_target_speed(double location, int flat_speed, int bottom);
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

	curve_pattern _curve;
	flat_pattern _flat;

	double _current_limit = no_pattern;
	int _current_bottom = no_pattern;
	handle_command _handle = handle_command::neutral();

	void update_pattern();
	void update_handle();

	template <typename OStream>
	friend OStream& operator<<(OStream& os, const pattern_generator& c) {
		if (!c._flat) {
			return os << "(inactive)";
		}
		os << "brake: " << c._handle << ", current: " << c._current_limit << " -> " << c._current_bottom
		   << ", curve: " << c._curve << ", flat: " << c._flat;
		return os;
	}
};
} // namespace turenar::koatc::pattern

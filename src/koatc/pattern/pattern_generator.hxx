#pragma once

#include <limits>
#include <spdlog/fmt/ostr.h>
#include "ats/vehicle_state.hxx"
#include "koatc/pattern/curve_pattern.hxx"
#include "koatc/pattern/flat_pattern.hxx"
#include "koatc/pattern/handle_command.hxx"

namespace turenar::koatc {
class configuration;
class init_requirements;
}

namespace turenar::koatc::pattern {
class pattern_generator {
public:
	pattern_generator(const init_requirements& req, bool use_emergency = false);

	void set_flat_speed(int speed);
	void set_target_speed(double location, int speed);
	void set_target_speed(double location, int flat_speed, int bottom);
	void clear();

	void tick();

	[[nodiscard]] bool active() const {
		return !_inactive_by_self && !_inactive_by_exclusive && !!_flat;
	}
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
	const configuration& _config;
	const bve::ats::vehicle_state& _vehicle_state;
	const pattern_generator* _exclusive_pattern = nullptr;

	curve_pattern _curve;
	flat_pattern _flat;

	double _current_limit = no_pattern;
	int _current_bottom = no_pattern;
	handle_command _handle = handle_command::neutral();

	bool _use_emergency = false;
	bool _inactive_by_self = false;
	bool _inactive_by_exclusive = false;

	void update_pattern();
	void update_handle();
	void update_handle_normal();
	void update_handle_emergency();
	void clear_handle();

	template <typename OStream>
	friend OStream& operator<<(OStream& os, const pattern_generator& c) {
		if (!c._flat) {
			return os << "(no pattern)";
		}
		if (c._inactive_by_self) {
			os << "(inactive:self) ";
		} else if (c._inactive_by_exclusive) {
			os << "(inactive:other) ";
		}
		os << "brake: " << c._handle << ", current: " << c._current_limit << " -> " << c._current_bottom
		   << ", curve: " << c._curve << ", flat: " << c._flat;
		return os;
	}
};
} // namespace turenar::koatc::pattern

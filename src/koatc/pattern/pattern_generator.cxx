#include <cmath>
#include <spdlog/spdlog.h>
#include "koatc/pattern/pattern_generator.hxx"

namespace turenar::koatc::pattern {
namespace {
	constexpr int full_brake_offset = 5;
	constexpr int emergency_offset = 10;
} // namespace

// km/h/s -> km/1000 h^2
pattern_generator::pattern_generator(const bve::ats::vehicle_state& state, double dec)
		: _vehicle_state(state), _curve(dec * 3.6) {}
void pattern_generator::clear() {
	_curve.clear();
	_flat.clear();
}
void pattern_generator::set_flat_speed(int speed) {
	_curve.clear();
	_flat.generate_pattern(speed);
}
void pattern_generator::set_target_speed(double location, int speed) {
	set_target_speed(location, speed, speed);
}
void pattern_generator::set_target_speed(double location, int flat_speed, int bottom) {
	_curve.generate_pattern(location, flat_speed, bottom);
	_flat.generate_pattern(flat_speed);
}
void pattern_generator::tick() {
	update_pattern();
	update_handle();
}
void pattern_generator::update_pattern() {
	if (_curve.active(_vehicle_state.location)) {
		_current_limit = _curve.limit_of(_vehicle_state.location);
		_current_bottom = _curve.bottom();
	} else {
		_current_limit = _flat.limit_of(_vehicle_state.location);
		_current_bottom = _flat.bottom();
	}
}
void pattern_generator::update_handle() {
	if (_vehicle_state.speed > _current_limit + emergency_offset) {
		_handle = handle_command::emergency();
	} else if (_vehicle_state.speed > _current_limit + full_brake_offset) {
		_handle = handle_command::full_brake();
	} else if (_vehicle_state.speed > _current_limit) {
		_handle = handle_command::half_brake();
	} else {
		_handle = handle_command::neutral();
	}
}
} // namespace turenar::koatc::pattern

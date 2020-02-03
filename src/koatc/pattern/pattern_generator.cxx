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
		: _vehicle_state(state), _deceleration(dec * 3.6) {}
void pattern_generator::clear() {
	_flat_start_location = minimum_location;
	_zero_location = maximum_location;
	_flat_bottom = no_pattern;
}
void pattern_generator::set_target_speed(double location, int speed) {
	// v^2 - w^2 = 2ax
	// x=(v^2-w^2)/2a   where w=0
	// a=km/1000 h^2, v=km/h, location=m
	_zero_location = location + (speed * speed) / _deceleration / 2.;
	spdlog::debug("loc: {}, zero: {}, speed: {}", location, _zero_location, speed);
	_flat_start_location = location;
	_curve_target = speed;
	_curve_bottom = speed;
	_flat_bottom = speed;
}
void pattern_generator::tick() {
	if (_vehicle_state.location > _flat_start_location) {
		_current_bottom = _flat_bottom;
		_current_limit = _flat_bottom;
	} else {
		// w^2 = v^2 + 2ax  where v=0
		// w=km/h, a=km/1000 h^2, x=m
		_current_limit = std::sqrt(2 * _deceleration * (_zero_location - _vehicle_state.location));
		spdlog::debug(
				"dclr: {}, zero: {}, loc: {}, limit: {}",
				_deceleration,
				_zero_location,
				_vehicle_state.location,
				_current_limit);
		_current_bottom = _curve_bottom;
	}

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

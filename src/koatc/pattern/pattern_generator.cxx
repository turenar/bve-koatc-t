#include <cmath>
#include "koatc/pattern/pattern_generator.hxx"

namespace turenar::koatc::pattern {
namespace {
	constexpr int emergency_offset = 10;
}

// km/h/s -> m/h^2
pattern_generator::pattern_generator(double deceleration) : _deceleration(deceleration * 3600 * 1000) {}
void pattern_generator::clear() {
	_flat_start_location = minimum_location;
	_zero_location = maximum_location;
	_flat_bottom = no_pattern;
}
void pattern_generator::set_target_speed(double location, int speed) {
	// v^2 - w^2 = 2ax
	// x=(v^2-w^2)/2a   where w=0
	// a=m/h^2, v=km/h, location=m
	int meter_per_second = speed * 1000;
	_zero_location = location + (meter_per_second * meter_per_second) / _deceleration / 2;
	_flat_start_location = location;
	_curve_target = speed;
	_curve_bottom = speed;
	_flat_bottom = speed;
}
void pattern_generator::update_location(double loc) {
	_location = loc;
	if (_location > _flat_start_location) {
		_current_bottom = _flat_bottom;
		_current_limit = _flat_bottom;
	} else {
		// w^2 = v^2 + 2ax  where v=0
		// w=km/h, a=m/h^2, x=m
		_current_limit = std::sqrt(2 * _deceleration * (_zero_location - _location)) / 1000;
		_current_bottom = _curve_bottom;
	}
}
double pattern_generator::emergency_limit() const {
	if (_use_emergency) {
		return _current_limit;
	} else {
		return _current_limit + emergency_offset;
	}
}
} // namespace turenar::koatc::pattern

#include <cmath>
#include <spdlog/spdlog.h>
#include "koatc/pattern/curve_pattern.hxx"

namespace turenar::koatc::pattern {
curve_pattern::curve_pattern(double dec) : _deceleration(dec) {}
void curve_pattern::generate_pattern(double location, int speed, int bottom) {
	// v^2 - w^2 = 2ax
	// x=(v^2-w^2)/2a   where w=0
	// a=km/1000 h^2, v=km/h, location=m
	_zero_location = location + (speed * speed) / _deceleration / 2.;
	_flat_start_location = location;
	_target = speed;
	_bottom = bottom;
}
void curve_pattern::clear() {
	_zero_location = maximum_location;
	_flat_start_location = minimum_location;
	_target = no_pattern;
	_bottom = no_pattern;
}
double curve_pattern::limit_of(double location) const {
	// w^2 = v^2 + 2ax  where v=0
	// w=km/h, a=km/1000 h^2, x=m
	return std::sqrt(2 * _deceleration * (_zero_location - location));
}
} // namespace turenar::koatc::pattern

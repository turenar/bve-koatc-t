#pragma once

#include "koatc/pattern/pattern_generator.hxx"

namespace turenar::koatc::pattern {
class speed_limit_pattern : public pattern_generator {
	using pattern_generator::pattern_generator;

public:
	void activate(double location, int flat_speed) {
		set_target_speed(location, flat_speed);
	}
};
} // namespace turenar::koatc::pattern

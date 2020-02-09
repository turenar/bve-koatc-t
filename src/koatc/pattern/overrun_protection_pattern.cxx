#include <spdlog/spdlog.h>
#include "koatc/pattern/overrun_protection_pattern.hxx"
#include "koatc/section/section_manager.hxx"
#include "koatc/signal/signal_manager.hxx"

namespace turenar::koatc::pattern {
namespace {
	constexpr int minimum_section_length = 30;
}
void overrun_protection_pattern::activate(int section, double section_location, double location, int speed) {
	if (section == 0) {
		_target = section_location + minimum_section_length;
		set_target_speed(location, speed);
	} else {
		_target = minimum_location;
		clear();
	}
}
void overrun_protection_pattern::tick() {
	if (_target > minimum_location) {
		const auto& red_section = _section_manager.get(_signal_manager.open_section());
		double red_location = red_section.start_location;
		if (_target < red_location) {
			_target = minimum_location;
			clear();
		}
	}

	pattern_generator::tick();
}
} // namespace turenar::koatc::pattern

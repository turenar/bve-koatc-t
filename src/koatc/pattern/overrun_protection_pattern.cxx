#include <spdlog/spdlog.h>
#include "koatc/pattern/init_requirements.hxx"
#include "koatc/pattern/overrun_protection_pattern.hxx"
#include "koatc/section/section_manager.hxx"
#include "koatc/signal/signal_manager.hxx"

namespace turenar::koatc::pattern {
namespace {
	constexpr int minimum_section_length = 30;
}
overrun_protection_pattern::overrun_protection_pattern(const init_requirements& req, bool use_emergency)
		: pattern_generator(req, use_emergency), _signal_manager(req.signal), _section_manager(req.section) {}
void overrun_protection_pattern::activate(double section_location, double location, int speed) {
	_target = section_location;
	set_target_speed(location, speed);
}
void overrun_protection_pattern::tick() {
	const auto& red_section = _section_manager.get(_signal_manager.open_section());
	double red_location = red_section.start_location;
	_inactive_by_self = std::abs(_target - red_location) >= minimum_section_length;
	pattern_generator::tick();
}
} // namespace turenar::koatc::pattern

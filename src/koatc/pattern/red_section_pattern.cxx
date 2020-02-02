#include <spdlog/spdlog.h>
#include "ats/vehicle_state.hxx"
#include "koatc/pattern/red_section_pattern.hxx"
#include "koatc/section/section_manager.hxx"
#include "koatc/signal/signal_manager.hxx"

namespace turenar::koatc::pattern {
namespace {
	constexpr double safety_pattern_offset = 15;
	constexpr double deceleration = 2.0;
} // namespace

red_section_pattern::red_section_pattern(
		const bve::ats::vehicle_state& state,
		const section::section_manager& section_manager,
		const signal::signal_manager& signal_manager)
		: pattern_generator(deceleration), _vehicle_state(state), _signal_manager(signal_manager),
		  _section_manager(section_manager) {}
void red_section_pattern::tick() {
	int open_section = _signal_manager.open_section();
	const section::section_info& section = _section_manager.get(open_section);
	spdlog::debug("section: {}", section.start_location);
	double zero_location = section.start_location;
	if (section.type != section::section_type::with_orp) {
		zero_location -= safety_pattern_offset;
	}
	set_target_speed(zero_location, 0);
}
} // namespace turenar::koatc::pattern

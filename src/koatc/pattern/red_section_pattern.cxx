#include <spdlog/spdlog.h>
#include "ats/vehicle_state.hxx"
#include "koatc/pattern/red_section_pattern.hxx"
#include "koatc/section/section_manager.hxx"
#include "koatc/signal/signal_manager.hxx"

namespace turenar::koatc::pattern {
namespace {
	constexpr double safety_pattern_offset = 15;
	constexpr double deceleration = 2000;
} // namespace

red_section_pattern::red_section_pattern(
		const bve::ats::vehicle_state& state,
		const section::section_manager& section_manager,
		const signal::signal_manager& signal_manager)
		: pattern_generator(deceleration), _vehicle_state(state), _signal_manager(signal_manager),
		  _section_manager(section_manager) {}
void red_section_pattern::update_vehicle_state(const bve::ats::vehicle_state& state) {
	pattern_generator::update_vehicle_state(state);

	int open_section = _signal_manager.open_section();
	const section::section_info& section = _section_manager.get(open_section);
	if (open_section <= 0) {
		_current_limit = 0;
		_handle.promote(handle_command::emergency());
	} else if (section.type == section::section_type::with_orp) {
		if (_handle) { // if activated
			_handle.promote(handle_command::emergency());
		}
	} else if (/* !with_orp && */ _current_limit <= 5) {
		if (0 <= _speed && _speed < std::numeric_limits<double>::epsilon()) {
			_handle.promote(handle_command::full_brake());
		} else {
			//			_current_limit = 5; // ?
			_handle.promote(handle_command::half_brake());
		}
	}
}
void red_section_pattern::tick() {
	int open_section = _signal_manager.open_section();
	const section::section_info& section = _section_manager.get(open_section);
	double zero_location = section.start_location;
	if (section.type != section::section_type::with_orp) {
		zero_location -= safety_pattern_offset;
	}
	set_target_speed(zero_location, 0);
}

} // namespace turenar::koatc::pattern

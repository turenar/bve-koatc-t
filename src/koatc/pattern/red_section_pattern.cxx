#include "ats/vehicle_state.hxx"
#include "koatc/pattern/red_section_pattern.hxx"
#include "koatc/section/section_manager.hxx"
#include "koatc/signal/signal_manager.hxx"

namespace turenar::koatc::pattern {
red_section_pattern::red_section_pattern(
		const bve::ats::vehicle_state& state,
		const section::section_manager& section_manager,
		const signal::signal_manager& signal_manager)
		: pattern_generator(state, deceleration), _signal_manager(signal_manager), _section_manager(section_manager) {}
void red_section_pattern::tick() {
	int open_section = _signal_manager.open_section();
	const section::section_info& section = _section_manager.get(open_section);
	double zero_location = section.start_location;
	if (section.type != section::section_type::with_orp) {
		zero_location -= safety_pattern_offset;
	}
	set_target_speed(zero_location, 0);

	// parent call
	pattern_generator::tick();

	if (open_section <= 0) {
		_current_limit = 0;
		_handle.promote(handle_command::emergency());
	} else if (section.type == section::section_type::with_orp) {
		if (_handle) { // if activated
			_handle.promote(handle_command::emergency());
		}
	} else if (/* !with_orp && */ _current_limit <= 5) {
		if (0 <= _vehicle_state.speed && _vehicle_state.speed < std::numeric_limits<double>::epsilon()) {
			_handle.promote(handle_command::full_brake());
		} else {
			//			_current_limit = 5; // ?
			_handle.promote(handle_command::half_brake());
		}
	}
}

} // namespace turenar::koatc::pattern

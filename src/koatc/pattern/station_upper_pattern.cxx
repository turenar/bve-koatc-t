#include "koatc/pattern/station_upper_pattern.hxx"
#include "koatc/station/station_manager.hxx"

namespace turenar::koatc::pattern {
station_upper_pattern::station_upper_pattern(
		const bve::ats::vehicle_state& state, const station::station_manager& manager)
		: pattern_generator(state, default_deceleration), _station_manager(manager) {}
void station_upper_pattern::tick() {
	if (_station_manager.has_stop() && _station_manager.next_stop().use_emergency()) {
		set_flat_speed(25);
	} else {
		clear();
	}

	pattern_generator::tick();
}
} // namespace turenar::koatc::pattern

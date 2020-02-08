#include "koatc/pattern/station_pattern.hxx"
#include "koatc/station/station_manager.hxx"

namespace turenar::koatc::pattern {
station_pattern::station_pattern(
		const configuration& config, const bve::ats::vehicle_state& state, const station::station_manager& manager)
		: pattern_generator(config, state), _station_manager(manager) {}
void station_pattern::tick() {
	if (_station_manager.has_stop()) {
		if (_station_manager.stop_arrived()) {
			set_flat_speed(5);
		} else {
			const auto& stop = _station_manager.next_stop();
			set_target_speed(stop.location(), stop.speed(), stop.bottom());
		}
	} else {
		clear();
	}

	pattern_generator::tick();

	if (_use_emergency && _handle) {
		_handle.promote(handle_command::emergency());
	}
}
} // namespace turenar::koatc::pattern

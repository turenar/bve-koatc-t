#include <spdlog/spdlog.h>
#include "koatc/pattern/station_pattern.hxx"
#include "koatc/station/station_manager.hxx"

namespace turenar::koatc::pattern {
station_pattern::station_pattern(const bve::ats::vehicle_state& state, const station::station_manager& manager)
		: pattern_generator(state, 2.), _station_manager(manager) {}
void station_pattern::tick() {
	if (_station_manager.has_stop()) {
		const auto& stop = _station_manager.next_stop();
		set_target_speed(stop.location(), stop.speed(), stop.bottom());
		_use_emergency = stop.use_emergency();
	} else {
		clear();
		_use_emergency = false;
	}

	pattern_generator::tick();

	if (_use_emergency && _handle) {
		_handle.promote(handle_command::emergency());
	}
}
} // namespace turenar::koatc::pattern

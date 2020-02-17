#include "koatc/pattern/init_requirements.hxx"
#include "koatc/pattern/station_pattern.hxx"
#include "koatc/station/station_manager.hxx"

namespace turenar::koatc::pattern {
station_pattern::station_pattern(const init_requirements& req)
		: pattern_generator(req), _station_manager(req.station) {}
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

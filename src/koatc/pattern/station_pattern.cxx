#include "koatc/pattern/init_requirements.hxx"
#include "koatc/pattern/overrun_protection_step1_pattern.hxx"
#include "koatc/pattern/station_pattern.hxx"
#include "koatc/station/station_manager.hxx"

namespace turenar::koatc::pattern {
station_pattern::station_pattern(const init_requirements& req, const overrun_protection_step1_pattern& pattern)
		: pattern_generator(req), _station_manager(req.station) {
	_exclusive_pattern = &pattern;
}
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

	//	pattern_generator::tick();
	update_pattern();
	update_handle();
	_inactive_by_exclusive =
			_exclusive_pattern && _exclusive_pattern->active() && _exclusive_pattern->limit() < limit();

	if (_use_emergency && _handle) {
		_handle.promote(handle_command::emergency());
	}
}
} // namespace turenar::koatc::pattern

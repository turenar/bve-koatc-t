#include "koatc/configuration.hxx"
#include "koatc/pattern/station_emergency_pattern.hxx"
#include "koatc/station/station_manager.hxx"

namespace turenar::koatc::pattern {
station_emergency_pattern::station_emergency_pattern(
		const configuration& config, const bve::ats::vehicle_state& state, const station::station_manager& manager)
		: pattern_generator(config, state, true), _station_manager(manager) {}
void station_emergency_pattern::tick() {
	if (_station_manager.is_next_station_stop() && _station_number == _station_manager.next_stop().number()) {
		if (_low_speed || _vehicle_state.speed < too_low_speed) {
			_low_speed = true;
			set_flat_speed(too_low_speed);
		}
	} else {
		_station_number = invalid_station;
		_low_speed = false;
		clear();
	}

	pattern_generator::tick();
}
bool station_emergency_pattern::buzzer() const {
	return _station_number != invalid_station && _current_limit <= 25 + _config.pattern_approaching() && !_low_speed;
}
void station_emergency_pattern::activate(double location, int speed) {
	if (_station_manager.is_next_station_stop()) {
		set_target_speed(location, speed);
		_station_number = _station_manager.next_stop().number();
		_low_speed = false;
	}
}
} // namespace turenar::koatc::pattern
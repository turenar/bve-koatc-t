#include "koatc/configuration.hxx"
#include "koatc/pattern/overrun_protection_step2_pattern.hxx"

namespace turenar::koatc::pattern {
void overrun_protection_step2_pattern::tick() {
	if (!active()) {
		_low_speed = false;
	} else if (_vehicle_state.speed < too_low_speed) {
		_low_speed = true;
		set_flat_speed(too_low_speed);
	}

	overrun_protection_pattern::tick();
}
bool overrun_protection_step2_pattern::buzzer() const {
	return active() && _current_limit <= 25 + _config.pattern_approaching() && !_low_speed;
}
} // namespace turenar::koatc::pattern

#include "koatc/handles.hxx"

namespace turenar::koatc {
handles::handles(
		const bve::ats::vehicle_spec& sp, notch_position brake, notch_position power, reverser_position reverser)
		: _spec(sp), _brake(brake), _power(power), _reverser(reverser) {
	this->brake(_brake);
}

void handles::cut_notch() {
	_power = notch_position{};
}

void handles::brake(notch_position brake) {
	if (brake && _brake < brake) {
		_brake = brake;
		_power = notch_position{};
	}
}

void handles::brake(pattern::handle_command command) {
	if (command == pattern::handle_command::half_brake()) {
		brake(_spec.brake_notches.half());
	} else if (command == pattern::handle_command::full_brake()) {
		brake(_spec.brake_notches);
	} else if (command == pattern::handle_command::emergency()) {
		brake(notch_position{static_cast<int>(_spec.brake_notches) + 1});
	}
}
void handles::neutral() {
	_reverser = reverser_position::neutral;
}
} // namespace turenar::koatc

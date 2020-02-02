#include <spdlog/spdlog.h>
#include "koatc/atc_manager.hxx"
#include "koatc/beacon_id.hxx"

namespace turenar::koatc {
namespace {}
atc_manager::atc_manager(bve::ats::vehicle_spec spec)
		: _spec(spec), _pattern_manager(_vehicle_state, _section_manager, _signal_manager) {}

bve::ats::handles atc_manager::tick(bve::ats::vehicle_state st, wrapper::atc_output output) {
	_vehicle_state = st;
	if (_timer.wake(st.time)) {
		_signal_manager.output(output);
		_pattern_manager.tick(output);
	}

	return bve::ats::handles{
			_brake_notch,
			_power_notch,
			_reverser,
			bve::ats::constant_speed_control::disable,
	};
}
void atc_manager::put_beacon(bve::ats::beacon beacon) {
	spdlog::info("beacon: type={}, distance={}, optional={}", beacon.type, beacon.distance, beacon.optional);
	switch (static_cast<beacon_id>(beacon.type)) {
	case beacon_id::section:
		if (_last_section_beacon_location != _vehicle_state.location) {
			_section_manager.clear();
			_last_section_beacon_location = _vehicle_state.location;
		}
		_section_manager.put_section(
				_vehicle_state.location + beacon.distance, static_cast<section::section_type>(beacon.optional));
		break;
	default:; // ignore
	}
}
} // namespace turenar::koatc

#include <spdlog/spdlog.h>
#include "koatc/atc_manager.hxx"
#include "koatc/beacon_id.hxx"
#include "koatc/handles.hxx"

namespace turenar::koatc {
namespace {}
atc_manager::atc_manager(bve::ats::vehicle_spec spec)
		: _spec(spec), _signal_manager(_vehicle_state),
		  _pattern_manager(_vehicle_state, _section_manager, _signal_manager) {}

bve::ats::handles atc_manager::tick(bve::ats::vehicle_state st, wrapper::atc_output output) {
	_vehicle_state = st;
	if (_timer.wake(st.time)) {
		_signal_manager.tick();
		_signal_manager.output(output);
		_pattern_manager.tick(output);

		if (_pattern_manager.handle() == pattern::handle_command::emergency()) {
			_signal_manager.emergency();
		}
	}

	handles handle{_spec, _brake_notch, _power_notch, _reverser};
	handle.brake(_pattern_manager.handle());
	return handle;
}
void atc_manager::put_beacon(bve::ats::beacon beacon) {
	spdlog::info(
			"beacon: @{} type={}, distance={}, optional={}",
			_vehicle_state.location,
			beacon.type,
			beacon.distance,
			beacon.optional);
	switch (static_cast<beacon_id>(beacon.type)) {
	case beacon_id::section:
		_section_manager.process_beacon(
				_vehicle_state.location, beacon.distance, static_cast<section::section_type>(beacon.optional));
		break;
	default:; // ignore
	}
}
} // namespace turenar::koatc

#include <spdlog/spdlog.h>
#include "koatc/atc_manager.hxx"
#include "koatc/beacon_id.hxx"
#include "koatc/handles.hxx"

namespace turenar::koatc {
namespace {
	constexpr int extract_distance(int p) {
		return p / 1000;
	}
	constexpr int extract_speed(int p) {
		return p % 1000;
	}
} // namespace
atc_manager::atc_manager(configuration config, bve::ats::vehicle_spec spec)
		: _spec(spec), _config(config), _signal_manager(_vehicle_state), _station_manager(_vehicle_state),
		  _pattern_manager(_config, _vehicle_state, _section_manager, _signal_manager, _station_manager) {}

bve::ats::handles atc_manager::tick(bve::ats::vehicle_state st, wrapper::atc_output output) {
	_vehicle_state = st;
	if (_timer.wake(st.time)) {
		_signal_manager.tick();
		_signal_manager.output(output);
		_pattern_manager.tick(output);
		_station_manager.tick(output);

		if (_pattern_manager.handle() == pattern::handle_command::emergency()) {
			_signal_manager.emergency();
		}
	}

	handles handle{_spec, _brake_notch, _power_notch, _reverser};
	handle.brake(_pattern_manager.handle());
	return handle;
}
void atc_manager::put_beacon(bve::ats::beacon beacon) {
	int optional = beacon.optional;
	spdlog::info(
			"beacon: @{} type={}, distance={}, optional={}",
			_vehicle_state.location,
			beacon.type,
			beacon.distance,
			optional);
	auto type = static_cast<beacon_id>(beacon.type);
	switch (type) {
	case beacon_id::section:
		_section_manager.process_beacon(
				_vehicle_state.location, beacon.distance, static_cast<section::section_type>(optional));
		break;
	case beacon_id::control_stop_emergency:
	case beacon_id::speed_limit_1:
	case beacon_id::speed_limit_2:
	case beacon_id::speed_limit_3:
	case beacon_id::speed_limit_4:
		_pattern_manager.process_beacon(beacon);
		break;
	case beacon_id::train_number_and_current_station:
		_station_manager.train_number(beacon.optional / 100);
		break;
	case beacon_id::approach_station:
		_station_manager.approach_station(static_cast<unsigned int>(optional / 100), optional % 100);
		break;
	case beacon_id::control_stop:
		_station_manager.control_stop(_vehicle_state.location + extract_distance(optional), extract_speed(optional), 0);
		break;
	case beacon_id::control_stop_to_25:
		_station_manager.control_stop(
				_vehicle_state.location + extract_distance(optional), extract_speed(optional), extract_speed(optional));
		break;
	case beacon_id::notify_stop:
		_station_manager.notify_stop();
		break;
	case beacon_id::arrive_stop:
		_station_manager.arrive_station();
		break;
	default:; // ignore
	}
}
} // namespace turenar::koatc

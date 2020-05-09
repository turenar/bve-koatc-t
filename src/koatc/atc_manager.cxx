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
		: _spec(spec), _config(config), _control_key(_config), _signal_manager(_config, _vehicle_state),
		  _station_manager(_vehicle_state),
		  _pattern_manager(_config, _vehicle_state, _section_manager, _signal_manager, _station_manager),
		  _timer(config.interval()) {}

bve::ats::handles atc_manager::tick(bve::ats::vehicle_state st, wrapper::atc_output output) {
	// first, update location
	_vehicle_state = st;
	// second, process beacons
	// beacon has `distance' relative to *current* location, not last location passed in #tick invocation
	for (const auto& beacon : _unprocessed_beacons) {
		process_beacon(beacon);
	}
	_unprocessed_beacons.clear();

	// third, update ATC
	if (_timer.wake(st.time)) {
		_signal_manager.tick();
		_signal_manager.output(output);
		_pattern_manager.tick(output);
		_station_manager.tick(output);
		this->output(output);

		if (_pattern_manager.handle() == pattern::handle_command::emergency()) {
			_signal_manager.emergency();
		}
	}

	// finally, output handle
	handles handle{_spec, _brake_notch, _power_notch, _reverser};
	handle.brake(_pattern_manager.handle());
	return handle;
}
void atc_manager::put_beacon(bve::ats::beacon beacon) {
	_unprocessed_beacons.emplace_back(beacon);
}
void atc_manager::key_down(bve::ats::key_code key) {
	switch (key) {
	case bve::ats::key_code::key_7:
		_control_key.turn_left();
		break;
	case bve::ats::key_code::key_8:
		_control_key.turn_right();
		break;
	default:; // do nothing
	}
}
void atc_manager::key_up(bve::ats::key_code) {}
void atc_manager::output(wrapper::atc_output output) {
	output.set_panel(panel_id::atc_power, _control_key.active());
}
void atc_manager::process_beacon(const bve::ats::beacon& beacon) {
	int optional = beacon.optional;
	auto type = static_cast<beacon_id>(beacon.type);
	spdlog::info(
			"beacon: @{} type={}({}), distance={}, optional={}, signal={}",
			_vehicle_state.location,
			beacon.type,
			type,
			beacon.distance,
			optional,
			beacon.signal);
	switch (type) {
	case beacon_id::section:
		_section_manager.process_beacon(
				_vehicle_state.location, beacon.distance, static_cast<section::section_type>(optional));
		break;
	case beacon_id::overrun_protection_step1:
	case beacon_id::overrun_protection_step2:
	case beacon_id::control_stop_emergency:
	case beacon_id::speed_limit_1:
	case beacon_id::speed_limit_2:
	case beacon_id::speed_limit_3:
	case beacon_id::speed_limit_4:
		_pattern_manager.process_beacon(beacon);
		break;
	case beacon_id::train_number_and_current_stop:
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

#pragma once

#include <vector>
#include "ats/beacon.hxx"
#include "ats/handles.hxx"
#include "ats/key.hxx"
#include "ats/notch_position.hxx"
#include "ats/reverser_position.hxx"
#include "ats/sound_control.hxx"
#include "ats/vehicle_spec.hxx"
#include "ats/vehicle_state.hxx"
#include "koatc/configuration.hxx"
#include "koatc/control_key_manager.hxx"
#include "koatc/pattern/pattern_manager.hxx"
#include "koatc/section/section_manager.hxx"
#include "koatc/signal/signal_manager.hxx"
#include "koatc/station/station_manager.hxx"
#include "koatc/timer.hxx"
#include "koatc/wrapper/atc_output.hxx"

namespace turenar::koatc {
class atc_manager {
public:
	explicit atc_manager(configuration config, bve::ats::vehicle_spec spec);

	void power(bve::ats::notch_position notch) {
		_power_notch = notch;
	}
	void brake(bve::ats::notch_position notch) {
		_brake_notch = notch;
	}
	void reverser(bve::ats::reverser_position rev) {
		_reverser = rev;
	}
	void key_down(bve::ats::key_code key);
	void key_up(bve::ats::key_code key);
	void put_beacon(bve::ats::beacon beacon);
	void set_open_section(int open_section) {
		_signal_manager.set_open_section(open_section);
	}

	bve::ats::handles tick(bve::ats::vehicle_state st, wrapper::atc_output output);

private:
	const bve::ats::vehicle_spec _spec;
	configuration _config;
	bve::ats::vehicle_state _vehicle_state = {};

	std::vector<bve::ats::beacon> _unprocessed_beacons;

	control_key_manager _control_key;
	signal::signal_manager _signal_manager;
	section::section_manager _section_manager;
	station::station_manager _station_manager;
	pattern::pattern_manager _pattern_manager;

	bve::ats::reverser_position _reverser = bve::ats::reverser_position::neutral;
	bve::ats::notch_position _power_notch{0};
	bve::ats::notch_position _brake_notch{0};

	timer _timer;

	void process_beacon(const bve::ats::beacon& beacon);
	void output(wrapper::atc_output output);
};
} // namespace turenar::koatc

#pragma once

#include "ats/handles.hxx"
#include "ats/notch_position.hxx"
#include "ats/reverser_position.hxx"
#include "ats/sound_control.hxx"
#include "ats/vehicle_spec.hxx"
#include "ats/vehicle_state.hxx"
#include "koatc/wrapper/panels.hxx"
#include "koatc/wrapper/sounds.hxx"

namespace turenar::koatc {
class atc_manager {
public:
	explicit atc_manager(bve::ats::vehicle_spec spec);

	void power(bve::ats::notch_position notch) {
		_power_notch = notch;
	}
	void brake(bve::ats::notch_position notch) {
		_brake_notch = notch;
	}
	void reverser(bve::ats::reverser_position rev) {
		_reverser = rev;
	}

	bve::ats::handles tick(bve::ats::vehicle_state st, wrapper::panels panels, wrapper::sounds sounds);

private:
	const bve::ats::vehicle_spec _spec;
	bve::ats::reverser_position _reverser = bve::ats::reverser_position::neutral;
	bve::ats::notch_position _power_notch{0};
	bve::ats::notch_position _brake_notch{0};
};
} // namespace turenar::koatc

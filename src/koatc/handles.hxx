#pragma once

#include "ats/handles.hxx"
#include "ats/vehicle_spec.hxx"
#include "koatc/pattern/handle_command.hxx"

namespace turenar::koatc {
class handles {
public:
	using notch_position = bve::ats::notch_position;
	using reverser_position = bve::ats::reverser_position;
	using constant_speed_control = bve::ats::constant_speed_control;

	handles(const bve::ats::vehicle_spec& sp, notch_position brake, notch_position power, reverser_position reverser);

	void cut_notch();
	void brake(notch_position brake);
	void brake(pattern::handle_command);

	operator bve::ats::handles() const { // NOLINT(google-explicit-constructor)
		return {_brake, _power, _reverser, _constant_speed_control};
	}

private:
	const bve::ats::vehicle_spec& _spec;
	notch_position _brake;
	notch_position _power;
	reverser_position _reverser;
	constant_speed_control _constant_speed_control;
};
} // namespace turenar::koatc

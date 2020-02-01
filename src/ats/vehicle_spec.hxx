#pragma once

#include "ats/atsplugin.hxx"
#include "ats/notch_position.hxx"

namespace bve::ats {
struct vehicle_spec {
	notch_position brake_notches; // Number of Brake Notches
	notch_position power_notches; // Number of Power Notches
	notch_position ats_notch; // ATS Cancel Notch
	notch_position b67_notch; // 80% Brake (67 degree)
	int cars; // Number of Cars

	vehicle_spec() = default;
	vehicle_spec(const ATS_VEHICLESPEC& v) // NOLINT(google-explicit-constructor)
			: brake_notches(v.BrakeNotches), power_notches(v.PowerNotches), ats_notch(v.AtsNotch),
			  b67_notch(v.B67Notch), cars(v.Cars) {}
	operator ATS_VEHICLESPEC() const { // NOLINT(google-explicit-constructor)
		ATS_VEHICLESPEC v{};
		v.BrakeNotches = static_cast<int>(brake_notches);
		v.PowerNotches = static_cast<int>(power_notches);
		v.AtsNotch = static_cast<int>(ats_notch);
		v.B67Notch = static_cast<int>(b67_notch);
		v.Cars = cars;
		return v;
	}
};
} // namespace bve::ats

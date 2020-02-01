#pragma once

#include "ats/atsplugin.hxx"

namespace bve::ats {
struct vehicle_state {
	double location; // Train Position (Z-axis) (m)
	float speed; // Train Speed (km/h)
	int time; // Time (ms)
	float brake_cylinder_pressure; // Pressure of Brake Cylinder (Pa)
	float main_reservoir_pressure; // Pressure of MR (Pa)
	float equalizing_air_reservoir_pressure; // Pressure of ER (Pa)
	float brake_pipe_pressure; // Pressure of BP (Pa)
	float supply_air_reservoir_pressure; // Pressure of SAP (Pa)
	float current; // Current (A)

	vehicle_state() = default;
	vehicle_state(const ATS_VEHICLESTATE& s)
			: location(s.Location), speed(s.Speed), time(s.Time), brake_cylinder_pressure(s.BcPressure),
			  main_reservoir_pressure(s.MrPressure), equalizing_air_reservoir_pressure(s.ErPressure),
			  brake_pipe_pressure(s.BpPressure), supply_air_reservoir_pressure(s.SapPressure), current(s.Current) {}
	operator ATS_VEHICLESTATE() const {
		ATS_VEHICLESTATE s{};
		s.Location = location;
		s.Speed = speed;
		s.Time = time;
		s.BcPressure = brake_cylinder_pressure;
		s.MrPressure = main_reservoir_pressure;
		s.ErPressure = equalizing_air_reservoir_pressure;
		s.BpPressure = brake_pipe_pressure;
		s.SapPressure = supply_air_reservoir_pressure;
		s.Current = current;
		return s;
	}
};
} // namespace bve::ats

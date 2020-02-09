#pragma once

#include "koatc/pattern/pattern_generator.hxx"
#include "koatc/wrapper/atc_output.hxx"

namespace turenar::koatc::station {
class station_manager;
}
namespace turenar::koatc::pattern {
class station_emergency_pattern : public pattern_generator {
private:
	constexpr static int invalid_station = -1;

public:
	station_emergency_pattern(const configuration&, const bve::ats::vehicle_state&, const station::station_manager&);

	void activate(double location, int speed);
	[[nodiscard]] bool buzzer() const {
		return _station_number != invalid_station && !_low_speed;
	}
	void tick();

private:
	const station::station_manager& _station_manager;
	int _station_number = invalid_station;
	bool _low_speed = false;
	bool _buzzer;
};
} // namespace turenar::koatc::pattern

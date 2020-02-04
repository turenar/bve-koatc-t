#pragma once

#include "koatc/pattern/pattern_generator.hxx"

namespace turenar::koatc::station {
class station_manager;
}
namespace turenar::koatc::pattern {
class station_pattern : public pattern_generator {
public:
	station_pattern(const bve::ats::vehicle_state&, const station::station_manager&);

	void tick();

private:
	const station::station_manager& _station_manager;
	bool _use_emergency = false;
};
} // namespace turenar::koatc::pattern

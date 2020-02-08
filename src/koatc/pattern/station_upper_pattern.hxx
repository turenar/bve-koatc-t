#pragma once

#include "koatc/pattern/pattern_generator.hxx"

namespace turenar::koatc::station {
class station_manager;
}
namespace turenar::koatc::pattern {
class station_upper_pattern : public pattern_generator {
public:
	station_upper_pattern(const configuration&, const bve::ats::vehicle_state&, const station::station_manager&);

	void tick();

private:
	const station::station_manager& _station_manager;
};
} // namespace turenar::koatc::pattern

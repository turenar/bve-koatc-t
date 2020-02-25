#pragma once

#include "koatc/pattern/pattern_generator.hxx"

namespace turenar::koatc::station {
class station_manager;
}
namespace turenar::koatc::pattern {
class overrun_protection_step1_pattern;

class station_pattern : public pattern_generator {
public:
	station_pattern(const init_requirements& req, const overrun_protection_step1_pattern& pattern);

	void tick();

private:
	const station::station_manager& _station_manager;
};
} // namespace turenar::koatc::pattern

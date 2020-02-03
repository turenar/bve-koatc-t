#pragma once

#include "koatc/pattern/pattern_generator.hxx"

namespace bve::ats {
struct vehicle_state;
}
namespace turenar::koatc {
namespace signal {
	class signal_manager;
}
namespace section {
	class section_manager;
}
} // namespace turenar::koatc
namespace turenar::koatc::pattern {
class red_section_pattern : public pattern_generator {
public:
	red_section_pattern(
			const bve::ats::vehicle_state& state,
			const section::section_manager& section_manager,
			const signal::signal_manager& signal_manager);

	void update_vehicle_state(const bve::ats::vehicle_state&);
	void tick();

private:
	const bve::ats::vehicle_state& _vehicle_state;
	const signal::signal_manager& _signal_manager;
	const section::section_manager& _section_manager;
};
} // namespace turenar::koatc::pattern

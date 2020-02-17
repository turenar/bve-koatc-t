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
class overrun_protection_step1_pattern;

class red_section_pattern : public pattern_generator {
public:
	red_section_pattern(const init_requirements& req, const overrun_protection_step1_pattern& orp);

	void tick();

private:
	const signal::signal_manager& _signal_manager;
	const section::section_manager& _section_manager;
};
} // namespace turenar::koatc::pattern

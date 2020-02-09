#pragma once

#include "koatc/pattern/overrun_protection_pattern.hxx"

namespace turenar::koatc::pattern {
class overrun_protection_step1_pattern : public overrun_protection_pattern {
public:
	overrun_protection_step1_pattern(
			const configuration& config,
			const bve::ats::vehicle_state& state,
			const signal::signal_manager& signal,
			const section::section_manager& section)
			: overrun_protection_pattern(config, state, signal, section, false) {}
};
} // namespace turenar::koatc::pattern

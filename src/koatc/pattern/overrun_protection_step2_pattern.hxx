#pragma once

#include "koatc/pattern/overrun_protection_pattern.hxx"

namespace turenar::koatc::pattern {
class overrun_protection_step2_pattern : public overrun_protection_pattern {
public:
	overrun_protection_step2_pattern(
			const configuration& config,
			const bve::ats::vehicle_state& state,
			const signal::signal_manager& signal,
			const section::section_manager& section)
			: overrun_protection_pattern(config, state, signal, section, true) {}

	void tick();

	[[nodiscard]] bool buzzer() const;

private:
	bool _low_speed;
};
} // namespace turenar::koatc::pattern

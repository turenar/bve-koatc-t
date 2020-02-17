#pragma once

#include "koatc/pattern/overrun_protection_pattern.hxx"

namespace turenar::koatc::pattern {
class overrun_protection_step2_pattern : public overrun_protection_pattern {
public:
	overrun_protection_step2_pattern(const init_requirements& req) : overrun_protection_pattern(req, true) {}

	void tick();

	[[nodiscard]] bool buzzer() const;

private:
	bool _low_speed;
};
} // namespace turenar::koatc::pattern

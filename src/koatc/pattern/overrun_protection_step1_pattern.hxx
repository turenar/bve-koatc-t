#pragma once

#include "koatc/pattern/overrun_protection_pattern.hxx"

namespace turenar::koatc::pattern {
class overrun_protection_step1_pattern : public overrun_protection_pattern {
public:
	overrun_protection_step1_pattern(const init_requirements& req) : overrun_protection_pattern(req, false) {}
};
} // namespace turenar::koatc::pattern

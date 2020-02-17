#pragma once

#include "koatc/pattern/pattern_generator.hxx"

namespace turenar::koatc {
namespace section {
	class section_manager;
}
namespace signal {
	class signal_manager;
}
} // namespace turenar::koatc
namespace turenar::koatc::pattern {
class overrun_protection_pattern : public pattern_generator {
public:
	void activate(double section_location, double location, int speed);
	void tick();

protected:
	const signal::signal_manager& _signal_manager;
	const section::section_manager& _section_manager;
	double _target = minimum_location;

	overrun_protection_pattern(const init_requirements& req, bool use_emergency);

	template <typename OStream>
	friend OStream& operator<<(OStream& os, const overrun_protection_pattern& c) {
		os << static_cast<const pattern_generator&>(c);
		if (c._flat) {
			os << ", section_location: @" << c._target;
		}
		return os;
	}
};
} // namespace turenar::koatc::pattern

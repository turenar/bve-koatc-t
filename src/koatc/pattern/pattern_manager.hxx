#pragma once

#include <type_traits>
#include "ats/handles.hxx"
#include "koatc/pattern/red_section_pattern.hxx"
#include "koatc/wrapper/atc_output.hxx"

namespace turenar::koatc::pattern {
class pattern_manager {
public:
	pattern_manager(const bve::ats::vehicle_state&, const section::section_manager&, const signal::signal_manager&);

	void tick(wrapper::atc_output);

	[[nodiscard]] handle_command handle() const{
		return _handle;
	}

private:
	red_section_pattern _red_section;
	const signal::signal_manager& _signal_manager;
	const bve::ats::vehicle_state& _vehicle_state;
	double _limit = 0;
	double _emergency_limit = 0;
	int _bottom = 0;
	handle_command _handle = handle_command::neutral();

	void update_pattern();
	void update_monitor(wrapper::atc_output);
	void debug_patterns() const;

	template <typename Accumulator, typename Result>
	Result accumulate_beacon(Result start, Accumulator op);
	template <typename UnaryFunction>
	void each_beacon(UnaryFunction fn);
};
} // namespace turenar::koatc::pattern

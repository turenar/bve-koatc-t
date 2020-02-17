#pragma once

#include <type_traits>
#include "ats/beacon.hxx"
#include "ats/handles.hxx"
#include "koatc/pattern/overrun_protection_step1_pattern.hxx"
#include "koatc/pattern/overrun_protection_step2_pattern.hxx"
#include "koatc/pattern/red_section_pattern.hxx"
#include "koatc/pattern/speed_limit_pattern.hxx"
#include "koatc/pattern/station_emergency_pattern.hxx"
#include "koatc/pattern/station_pattern.hxx"
#include "koatc/sound_player.hxx"
#include "koatc/wrapper/atc_output.hxx"

namespace turenar::koatc {
class configuration;
}

namespace turenar::koatc::pattern {
class init_requirements;

class pattern_manager {
public:
	pattern_manager(
			const configuration& config,
			const bve::ats::vehicle_state&,
			const section::section_manager&,
			const signal::signal_manager&,
			const station::station_manager&);

	void tick(wrapper::atc_output);

	[[nodiscard]] handle_command handle() const {
		return _handle;
	}

	void process_beacon(const bve::ats::beacon& beacon);

private:
	const configuration& _config;
	const signal::signal_manager& _signal_manager;
	const bve::ats::vehicle_state& _vehicle_state;

	red_section_pattern _red_section;
	speed_limit_pattern _speed_limits[4];
	station_pattern _station;
	station_emergency_pattern _station_emergency;
	overrun_protection_step1_pattern _orp_step1;
	overrun_protection_step2_pattern _orp_step2;

	sound_player _buzzer_player{sound_id::buzzer};

	double _limit = 0;
	int _bottom = 0;
	handle_command _handle = handle_command::neutral();
	bool _bell = false;

	pattern_manager(init_requirements);
	void update_pattern();
	void update_monitor(wrapper::atc_output);
	void debug_patterns() const;

	template <typename Accumulator, typename Result>
	Result accumulate_beacon(Result start, Accumulator op);
	template <typename UnaryFunction>
	void each_beacon(UnaryFunction fn);
	template <typename Pattern, typename... Args>
	void activate_beacon(Pattern& pattern, const bve::ats::beacon&, Args&&... args);
	[[nodiscard]] bool should_skip_bell() const;
};
} // namespace turenar::koatc::pattern

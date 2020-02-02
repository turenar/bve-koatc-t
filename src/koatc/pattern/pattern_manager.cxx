#include <bitset>
#include <tuple>
#include <ats/vehicle_state.hxx>
#include <spdlog/spdlog.h>
#include "koatc/pattern/pattern_manager.hxx"

namespace turenar::koatc::pattern {
namespace {
	constexpr int atc_monitor_speeds[] = {0,  5,  10, 15, 20, 25, 30, 35, 40,  45,  50,  55,
										  60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 120};
	constexpr int advance_notice = 10;
} // namespace

pattern_manager::pattern_manager(
		const bve::ats::vehicle_state& state,
		const turenar::koatc::section::section_manager& section,
		const turenar::koatc::signal::signal_manager& signal)
		: _red_section(state, section, signal), _signal_manager(signal), _vehicle_state(state) {}
template <typename UnaryFunction>
void pattern_manager::each_beacon(UnaryFunction fn) {
	fn(_red_section);
}
template <typename Accumulator, typename Result>
Result pattern_manager::accumulate_beacon(Result start, Accumulator op) {
	Result ret = std::move(start);
	each_beacon([&op, &ret](auto&& generator) mutable { ret = op(ret, generator); });
	return ret;
}
void pattern_manager::tick(wrapper::atc_output output) {
	update_monitor(output);
	update_pattern();
	debug_patterns();
}
void pattern_manager::update_monitor(wrapper::atc_output output) {
	int panel_base = static_cast<int>(panel_id::atc_speed_min_0);
	for (std::size_t i = 0; i < std::size(atc_monitor_speeds); ++i) {
		int speed = atc_monitor_speeds[i];
		bool needle;
		if (speed < _bottom) {
			needle = false;
		} else {
			needle = speed <= _limit;
		}
		output.set_panel(static_cast<panel_id>(panel_base + i), static_cast<int>(needle));
	}
}
void pattern_manager::update_pattern() {
	double location = _vehicle_state.location;
	each_beacon([location](auto&& pat) {
		pat.update_location(location);
		pat.tick();
	});
	double limit;
	double emergency_limit;
	std::tie(limit, emergency_limit) = accumulate_beacon(
			std::make_tuple(pattern_generator::no_pattern, pattern_generator::no_pattern),
			[](std::tuple<double, double> last, const pattern_generator& pat) {
				return std::make_tuple(
						std::min(std::get<0>(last), pat.normal_limit()),
						std::min(std::get<1>(last), pat.normal_limit()));
			});
	int bottom = accumulate_beacon(static_cast<int>(limit), [limit, location](int last, const pattern_generator& pat) {
		if (pat.normal_limit() > limit + advance_notice) {
			return last;
		} else {
			return std::min(last, pat.bottom());
		}
	});
	_limit = limit;
	_emergency_limit = emergency_limit;
	_bottom = bottom;
}
void pattern_manager::debug_patterns() const {
	spdlog::debug("patterns:");
	spdlog::debug("  section: {}", _red_section);
}
} // namespace turenar::koatc::pattern

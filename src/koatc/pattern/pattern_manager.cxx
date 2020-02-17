#include <bitset>
#include <tuple>
#include <spdlog/spdlog.h>
#include "ats/vehicle_state.hxx"
#include "koatc/beacon_id.hxx"
#include "koatc/pattern/init_requirements.hxx"
#include "koatc/pattern/pattern_manager.hxx"

namespace turenar::koatc::pattern {
namespace {
	constexpr int atc_monitor_speeds[] = {0,  5,  10, 15, 20, 25, 30, 35, 40,  45,  50,  55,
										  60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 120};
	constexpr int advance_notice = 10;
	constexpr int updated_bell_threshold = 2;

	constexpr int extract_distance(int p) {
		return p / 1000;
	}
	constexpr int extract_speed(int p) {
		return p % 1000;
	}
} // namespace

pattern_manager::pattern_manager(
		const configuration& config,
		const bve::ats::vehicle_state& state,
		const section::section_manager& section,
		const signal::signal_manager& signal,
		const station::station_manager& station)
		: pattern_manager{init_requirements{config, state, section, signal, station}} {}
pattern_manager::pattern_manager(turenar::koatc::pattern::init_requirements req)
		: _config(req.config), _signal_manager(req.signal),
		  _vehicle_state(req.state), _speed_limits{{req}, {req}, {req}, {req}}, _station(req), _station_emergency(req),
		  _orp_step1(req), _orp_step2(req), _red_section(req, _orp_step1) {}
template <typename UnaryFunction>
void pattern_manager::each_beacon(UnaryFunction fn) {
	for (auto& pat : _speed_limits) {
		fn(pat);
	}
	fn(_orp_step1);
	fn(_orp_step2);
	fn(_station);
	fn(_station_emergency);
	fn(_red_section);
}
template <typename Accumulator, typename Result>
Result pattern_manager::accumulate_beacon(Result start, Accumulator op) {
	Result ret = std::move(start);
	each_beacon([&op, &ret](auto&& generator) mutable { ret = op(ret, generator); });
	return ret;
}
template <typename Pattern, typename... Args>
void pattern_manager::activate_beacon(Pattern& pattern, const bve::ats::beacon& beacon, Args&&... args) {
	int distance = beacon.optional / 1000;
	int speed = beacon.optional % 1000;
	pattern.activate(std::forward<Args>(args)..., _vehicle_state.location + distance, speed);
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

	output.set_panel(panel_id::atc_normal_braking, static_cast<int>(_handle && _handle != handle_command::emergency()));
	output.set_panel(panel_id::atc_emergency_braking, static_cast<int>(_handle == handle_command::emergency()));

	if (_bell) {
		output.set_sound(sound_id::bell, bve::ats::sound_control::play);
		_bell = false;
	}
	if (_orp_step2.buzzer() || _station_emergency.buzzer()) {
		_buzzer_player.start();
	} else {
		_buzzer_player.stop();
	}
	_buzzer_player.output(output, _vehicle_state.time);
}
void pattern_manager::update_pattern() {
	using limit_bottom_pair = std::pair<double, int>;
	double location = _vehicle_state.location;
	double limit;
	int bottom;
	each_beacon([this](auto&& pat) { pat.tick(); });
	std::tie(limit, bottom) = accumulate_beacon(
			limit_bottom_pair{double{no_pattern}, no_pattern}, [](const limit_bottom_pair& last, const auto& pat) {
				if (pat.active() && pat.limit() < last.first) {
					return limit_bottom_pair{pat.limit(), pat.bottom()};
				} else {
					return last;
				}
			});
	bottom = accumulate_beacon(static_cast<int>(limit), [limit, location](int last, const auto& pat) {
		if (pat.active() && pat.limit() <= limit + advance_notice) {
			return std::min(last, pat.bottom());
		} else {
			return last;
		}
	});
	handle_command handle =
			accumulate_beacon(handle_command::neutral(), [](handle_command last, const auto& pat) -> handle_command {
				if (pat.active()) {
					last.promote(pat.handle());
				}
				return last;
			});

	if (_limit + updated_bell_threshold <= limit && !should_skip_bell()) {
		_bell = true;
	}
	_limit = limit;
	_bottom = bottom;
	_handle = handle;
}
bool pattern_manager::should_skip_bell() const {
	return false;
	//	return (std::abs(_limit) > too_low_speed || _vehicle_state.speed > std::numeric_limits<double>::epsilon());
}
void pattern_manager::debug_patterns() const {
	spdlog::debug("patterns: brake={}, current@{}={} -> {}", _handle, _vehicle_state.location, _limit, _bottom);
	spdlog::debug("  [section] {}", _red_section);
	for (auto& pat : _speed_limits) {
		spdlog::debug("  [speed]   {}", pat);
	}
	spdlog::debug("  [orp1]    {}", _orp_step1);
	spdlog::debug("  [orp2]    {}", _orp_step2);
	spdlog::debug("  [station] {}", _station);
	spdlog::debug("  [sta_emg] {}", _station_emergency);
}
void pattern_manager::process_beacon(const bve::ats::beacon& beacon) {
	switch (static_cast<beacon_id>(beacon.type)) {
	case beacon_id::speed_limit_1:
	case beacon_id::speed_limit_2:
	case beacon_id::speed_limit_3:
	case beacon_id::speed_limit_4: {
		speed_limit_pattern& limiter = _speed_limits[beacon.type - static_cast<int>(beacon_id::speed_limit_1)];
		activate_beacon(limiter, beacon);
	} break;
	case beacon_id::overrun_protection_step1:
		activate_beacon(_orp_step1, beacon, _vehicle_state.location + beacon.distance);
		break;
	case beacon_id::overrun_protection_step2:
		activate_beacon(_orp_step2, beacon, _vehicle_state.location + beacon.distance);
		if (!_orp_step2.active()) {
			break; // do not generate emergency pattern
		}
		[[fallthrough]];
	case beacon_id::control_stop_emergency:
		activate_beacon(_station_emergency, beacon);
		_station_emergency.activate(
				_vehicle_state.location + extract_distance(beacon.optional), extract_speed(beacon.optional));
		break;
	default:; // ignore
	}
}
} // namespace turenar::koatc::pattern

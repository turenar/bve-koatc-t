#include <bitset>
#include <tuple>
#include <spdlog/spdlog.h>
#include "ats/vehicle_state.hxx"
#include "koatc/beacon_id.hxx"
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
		: _config(config), _signal_manager(signal), _vehicle_state(state), _red_section(config, state, section, signal),
		  _speed_limits{{config, state}, {config, state}, {config, state}, {config, state}},
		  _station(config, state, station), _station_emergency(config, state, station) {}
template <typename UnaryFunction>
void pattern_manager::each_beacon(UnaryFunction fn) {
	fn(_red_section);
	for (auto& pat : _speed_limits) {
		fn(pat);
	}
	fn(_station);
	fn(_station_emergency);
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

	output.set_panel(panel_id::atc_normal_braking, static_cast<int>(_handle && _handle != handle_command::emergency()));
	output.set_panel(panel_id::atc_emergency_braking, static_cast<int>(_handle == handle_command::emergency()));

	if (_bell) {
		output.set_sound(sound_id::bell, bve::ats::sound_control::play);
		_bell = false;
	}
}
void pattern_manager::update_pattern() {
	double location = _vehicle_state.location;
	each_beacon([this](auto&& pat) { pat.tick(); });
	double limit = accumulate_beacon(static_cast<double>(no_pattern), [](double last, const pattern_generator& pat) {
		spdlog::debug("{}, {}", last, pat.limit());
		return std::min(last, pat.limit());
	});
	int bottom = accumulate_beacon(static_cast<int>(limit), [limit, location](int last, const pattern_generator& pat) {
		spdlog::debug("limit={}, bottom={}, pat_limit={}, pat_bottom={}", limit, last, pat.limit(), pat.bottom());
		if (pat.limit() > limit + advance_notice) {
			return last;
		} else {
			return std::min(last, pat.bottom());
		}
	});
	handle_command handle =
			accumulate_beacon(handle_command::neutral(), [](handle_command last, const auto& pat) -> handle_command {
				last.promote(pat.handle());
				return last;
			});

	if (_limit + updated_bell_threshold <= limit) {
		_bell = true;
	}
	_limit = limit;
	_bottom = bottom;
	_handle = handle;
}
void pattern_manager::debug_patterns() const {
	spdlog::debug("patterns: brake={}, current={} -> {}", _handle, _limit, _bottom);
	spdlog::debug("  [section] {}", _red_section);
	for (auto& pat : _speed_limits) {
		spdlog::debug("  [speed]   {}", pat);
	}
	spdlog::debug("  [station] {}", _station);
	spdlog::debug("  [sta_up]  {}", _station_emergency);
}
void pattern_manager::process_beacon(const bve::ats::beacon& beacon) {
	switch (static_cast<beacon_id>(beacon.type)) {
	case beacon_id::speed_limit_1:
	case beacon_id::speed_limit_2:
	case beacon_id::speed_limit_3:
	case beacon_id::speed_limit_4: {
		speed_limit_pattern& limiter = _speed_limits[beacon.type - static_cast<int>(beacon_id::speed_limit_1)];
		int distance = beacon.optional / 1000;
		int speed = beacon.optional % 1000;
		limiter.set_target_speed(_vehicle_state.location + distance, speed);
	} break;
	case beacon_id::control_stop_emergency:
		_station_emergency.activate(
				_vehicle_state.location + extract_distance(beacon.optional), extract_speed(beacon.optional));
		break;
	default:; // ignore
	}
}
} // namespace turenar::koatc::pattern

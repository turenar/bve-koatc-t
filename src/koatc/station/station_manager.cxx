#include <algorithm>
#include <limits>
#include <spdlog/spdlog.h>
#include "koatc/station/station_manager.hxx"

namespace turenar::koatc::station {
namespace {
	constexpr int invalid_station_for_monitor = 99;
} // namespace

station_manager::station_manager(const bve::ats::vehicle_state& state) : _vehicle_state(state) {}
void station_manager::train_number(unsigned int n) {
	_train_number = n;
	_operation_type = n / 1000;
}
void station_manager::tick(wrapper::atc_output output) {
	if (_stop_arrived) {
		if (std::abs(_vehicle_state.speed) > std::numeric_limits<double>::epsilon()) {
			_stopped_timer.reset(_vehicle_state.time);
		} else if (_stopped_timer.tick(_vehicle_state.time)) {
			_stop_approaching = false;
			_stop_arrived = false;
			_stations.erase(_stations.begin());
		}
	}

	output.set_panel(panel_id::operation_type, static_cast<int>(_operation_type % 10));
	output.set_panel(panel_id::atc_operation_type, static_cast<int>(_operation_type));
	if (is_next_station_stop() && _stop_approaching) {
		output.set_panel(panel_id::atc_next_stop, next_stop().number());
		_approaching_sound.output(output, _vehicle_state.time);
	} else {
		output.set_panel(panel_id::atc_next_stop, invalid_station_for_monitor);
	}
}
bool station_manager::is_next_station_stop() const {
	return !_stations.empty() && _stations.front().stop();
}
const station_info& station_manager::next_stop() const {
	auto it = std::find_if(_stations.cbegin(), _stations.cend(), [](const station_info& sta) { return sta.stop(); });
	assert(it != _stations.cend());
	return *it;
}
bool station_manager::has_stop() const {
	auto it = std::find_if(_stations.cbegin(), _stations.cend(), [](const station_info& sta) { return sta.stop(); });
	return it != _stations.cend();
}
void station_manager::approach_station(unsigned int stop_bits, int station) {
	unsigned int type = _operation_type % 10;
	bool stop = 0 != (stop_bits & (1u << type));
	if (!_stations.empty() && _stations.front().number() == station) {
		spdlog::warn("duplicated station {}, location jumped?", station);
	} else {
		spdlog::debug("approaching station {}, stop={}", station, stop);
		if (!_stations.empty()) {
			spdlog::info("current registered stations:");
			for (const auto& sta : _stations) {
				spdlog::info("  station={}, stop={}", sta.number(), sta.stop());
			}
		}
		_stations.emplace_back(station, stop);
	}
}
void station_manager::control_stop(double location, int speed, int bottom) {
	if (_stations.empty()) {
		spdlog::warn("no station registered! @{}", _vehicle_state.location);
	} else if (_next_control_station_index < 0 || _stations.size() <= std::size_t(_next_control_station_index)) {
		spdlog::warn("wrong station index: {} @{}", _next_control_station_index, _vehicle_state.location);
	} else {
		_stations[_next_control_station_index].control_stop(location, speed, bottom);
	}
	_next_control_station_index = 0;
}
void station_manager::notify_stop() {
	if (_stations.empty()) {
		spdlog::warn("no station registered! @{}", _vehicle_state.location);
	} else if (_stations.front().stop()) {
		_stop_approaching = true;
		_approaching_sound.start(_vehicle_state.time, 836 * 3);
	}
}
void station_manager::arrive_station() {
	if (_stations.empty()) {
		spdlog::warn("no station registered! @{}", _vehicle_state.location);
	} else if (_stations.front().stop()) {
		_stop_arrived = true;
	} else {
		_stations.erase(_stations.begin());
	}
}
} // namespace turenar::koatc::station

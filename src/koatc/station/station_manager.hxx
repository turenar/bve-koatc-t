#pragma once

#include <vector>
#include "ats/vehicle_state.hxx"
#include "koatc/station/station_info.hxx"
#include "koatc/timer.hxx"
#include "koatc/wrapper/atc_output.hxx"

namespace turenar::koatc::station {
class station_manager {
public:
	constexpr static int invalid_station = -1;

	explicit station_manager(const bve::ats::vehicle_state&);

	void train_number(unsigned int n);

	[[nodiscard]] bool is_next_station_stop() const;
	[[nodiscard]] bool has_stop() const;
	[[nodiscard]] const station_info& next_stop() const;
	[[nodiscard]] bool stop_arrived() const {
		return _stop_arrived;
	}

	void tick(wrapper::atc_output);

	void approach_station(unsigned int stop_bits, int station);
	void next_control_station(int index);
	void control_stop(double location, int speed, int bottom);
	void notify_stop();
	void arrive_station();

private:
	const bve::ats::vehicle_state& _vehicle_state;
	std::vector<station_info> _stations;
	timer _approaching_timer{836 * 3};
	timer _stopped_timer{5000};

	unsigned int _train_number = 0;
	unsigned int _operation_type = 0;
	//	int _last_stop = invalid_station;
	//	int _next_stop = invalid_station;
	//	int _next_next_stop = invalid_station;
	int _next_control_station_index = 0;
	bool _stop_approaching = false;
	bool _stop_arrived = false;
};
} // namespace turenar::koatc::station

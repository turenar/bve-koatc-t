#pragma once

#include "koatc/signal/car_signal.hxx"
#include "koatc/timer.hxx"
#include "koatc/wrapper/atc_output.hxx"

namespace bve::ats {
struct vehicle_state;
}
namespace turenar::koatc {
class configuration;
}

namespace turenar::koatc::signal {
class signal_manager {
public:
	signal_manager(const configuration& config, const bve::ats::vehicle_state&);

	void set_open_section(int);
	[[nodiscard]] car_signal signal() const {
		return _signal;
	}
	[[nodiscard]] int open_section() const;
	void emergency();
	void tick();
	void output(wrapper::atc_output output);

private:
	const bve::ats::vehicle_state& _vehicle_state;
	timer _emergency_timer;
	int _open_section = 0;
	car_signal _signal = car_signal::no_signal;
	car_signal _next_signal = _signal;
	bool _emergency = false;
};
} // namespace turenar::koatc::signal

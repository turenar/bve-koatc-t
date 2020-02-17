#include <spdlog/spdlog.h>
#include "ats/vehicle_state.hxx"
#include "koatc/configuration.hxx"
#include "koatc/signal/signal_manager.hxx"

namespace turenar::koatc::signal {
signal_manager::signal_manager(const configuration& config, const bve::ats::vehicle_state& state)
		: _vehicle_state(state), _emergency_timer(config.emergency_time()) {}
void signal_manager::set_open_section(int open_section) {
	spdlog::info("update open section: {}", open_section);
	_open_section = open_section;
	car_signal new_signal;
	if (open_section == 0) {
		new_signal = car_signal::no_signal;
	} else if (open_section == 1) {
		new_signal = car_signal::close;
	} else {
		new_signal = car_signal::open;
	}
	_next_signal = new_signal;
}
int signal_manager::open_section() const {
	if (_emergency) {
		return 0;
	} else {
		return _open_section;
	}
}
void signal_manager::tick() {
	if (std::abs(_vehicle_state.speed) >= std::numeric_limits<double>::epsilon()) {
		_emergency_timer.reset(_vehicle_state.time);
	} else if (_emergency && _emergency_timer.wake(_vehicle_state.time)) {
		_emergency = false;
	}
}
void signal_manager::output(wrapper::atc_output output) {
	car_signal signal = _emergency ? car_signal::no_signal : _next_signal;
	output.set_panel(panel_id::atc_signal_forbidden, static_cast<int>(signal == car_signal::no_signal));
	output.set_panel(panel_id::atc_signal_red, static_cast<int>(signal == car_signal::close));
	output.set_panel(panel_id::atc_signal_green, static_cast<int>(signal == car_signal::open));

	if (_signal != signal) {
		output.set_sound(sound_id::bell, bve::ats::sound_control::play);
		_signal = signal;
	}
}
void signal_manager::emergency() {
	_emergency = true;
}
} // namespace turenar::koatc::signal

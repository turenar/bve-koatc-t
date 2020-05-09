#include <spdlog/spdlog.h>
#include "ats/vehicle_state.hxx"
#include "koatc/configuration.hxx"
#include "koatc/signal/signal_manager.hxx"

namespace turenar::koatc::signal {
signal_manager::signal_manager(const configuration& config, const bve::ats::vehicle_state& state)
		: _vehicle_state(state), _emergency_timer(config.emergency_time()), _power_on_timer(config.power_on_time()) {}
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
	if (_emergency || _power_on_timer.active()) {
		return 0;
	} else {
		return _open_section;
	}
}
void signal_manager::tick() {
	_power_on_timer.tick(_vehicle_state.time);
	_emergency_timer.tick(_vehicle_state.time);
	if (std::abs(_vehicle_state.speed) >= std::numeric_limits<double>::epsilon()) {
		_emergency_timer.activate(_vehicle_state.time);
	} else if (_emergency && !_emergency_timer.active()) {
		_emergency = false;
	}
}
void signal_manager::output(wrapper::atc_output output) {
	car_signal signal = current_signal();

	output.set_panel(panel_id::atc_active, static_cast<int>(!_power_on_timer.active()));
	output.set_panel(panel_id::atc_signal_forbidden, static_cast<int>(signal == car_signal::no_signal));
	output.set_panel(panel_id::atc_signal_red, static_cast<int>(signal == car_signal::close));
	output.set_panel(panel_id::atc_signal_green, static_cast<int>(signal == car_signal::open));

	if (_signal != signal) {
		output.set_sound(sound_id::bell, bve::ats::sound_control::play);
		_signal = signal;
	}
}
void signal_manager::activate() {
	_power_on_timer.activate(_vehicle_state.time);
}
void signal_manager::shutdown() {
	// _power_on_timer.active() is atc "inactive"
	_power_on_timer.activate(std::numeric_limits<int>::max() - 1);
}
void signal_manager::emergency() {
	_emergency = true;
}
car_signal signal_manager::current_signal() const {
	if (_emergency || _power_on_timer.active()) {
		return car_signal::no_signal;
	} else {
		return _next_signal;
	}
}
} // namespace turenar::koatc::signal

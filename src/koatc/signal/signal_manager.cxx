#include <spdlog/spdlog.h>
#include "koatc/signal/signal_manager.hxx"

namespace turenar::koatc::signal {
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
	if (new_signal != _signal) {
		_bell = true;
	}
	_signal = new_signal;
}
void signal_manager::output(wrapper::atc_output output) {
	output.set_panel(panel_id::atc_signal_forbidden, static_cast<int>(_signal == car_signal::no_signal));
	output.set_panel(panel_id::atc_signal_red, static_cast<int>(_signal == car_signal::close));
	output.set_panel(panel_id::atc_signal_green, static_cast<int>(_signal == car_signal::open));

	if (_bell) {
		output.set_sound(sound_id::bell, bve::ats::sound_control::play);
		_bell = false;
	}
}
} // namespace turenar::koatc::signal

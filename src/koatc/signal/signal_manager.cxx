#include "koatc/signal/signal_manager.hxx"
#include <spdlog/spdlog.h>

namespace turenar::koatc::signal {
void signal_manager::set_open_section(int open_section) {
	spdlog::info("update open section: {}", open_section);
	_open_section = open_section;
	if (open_section == 0) {
		_signal = car_signal::no_signal;
	} else if (open_section == 1) {
		_signal = car_signal::close;
	} else {
		_signal = car_signal::open;
	}
}
void signal_manager::update_panel(wrapper::atc_output output) {
	output.set_panel(panel_id::atc_signal_forbidden, static_cast<int>(_signal == car_signal::no_signal));
	output.set_panel(panel_id::atc_signal_red, static_cast<int>(_signal == car_signal::close));
	output.set_panel(panel_id::atc_signal_green, static_cast<int>(_signal == car_signal::open));
}
} // namespace turenar::koatc::signal

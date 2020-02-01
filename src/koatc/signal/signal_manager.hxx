#pragma once

#include "koatc/signal/car_signal.hxx"
#include "koatc/wrapper/atc_output.hxx"

namespace turenar::koatc::signal {
class signal_manager {
public:
	void set_open_section(int);
	[[nodiscard]] car_signal signal() const {
		return _signal;
	}
	void output(wrapper::atc_output output);

private:
	int _open_section = 0;
	car_signal _signal = car_signal::no_signal;
	bool _bell = false;
};
} // namespace turenar::koatc::signal

#include "koatc/atc_manager.hxx"

namespace turenar::koatc {
atc_manager::atc_manager(bve::ats::vehicle_spec spec) : _spec(spec) {}

bve::ats::handles atc_manager::tick(bve::ats::vehicle_state st, wrapper::atc_output output) {
	_signal_manager.output(output);

	return bve::ats::handles{
			_brake_notch,
			_power_notch,
			_reverser,
			bve::ats::constant_speed_control::disable,
	};
}
} // namespace turenar::koatc

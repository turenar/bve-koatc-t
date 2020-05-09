#include <spdlog/spdlog.h>
#include "koatc/configuration.hxx"
#include "koatc/control_key_manager.hxx"
#include "koatc/wrapper/atc_output.hxx"

namespace turenar::koatc {
namespace {
	void update_position(int& position, const configuration& config, bool reset, int delta);
}

control_key_manager::control_key_manager(const turenar::koatc::configuration& config)
		: _config(config), _current_position(0) {}
void control_key_manager::turn_left() {
	update_position(_current_position, _config, _current_position > 0, -1);
}
void control_key_manager::turn_right() {
	update_position(_current_position, _config, _current_position < 0, 1);
}
bool control_key_manager::active() const {
	return _current_position == _config.control_key_position();
}
void control_key_manager::output(wrapper::atc_output output) const {
	output.set_panel(panel_id::atc_power, active());
}

namespace {
	void update_position(int& position, const configuration& config, bool reset, int delta) {
		if (reset) {
			position = 0;
		} else {
			position = std::clamp(position + delta, config.control_key_min(), config.control_key_max());
		}
	}
} // namespace
} // namespace turenar::koatc

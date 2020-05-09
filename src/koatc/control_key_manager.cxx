#include <spdlog/spdlog.h>
#include "koatc/configuration.hxx"
#include "koatc/control_key_manager.hxx"

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
	spdlog::debug(
			"position={}, min={}, max={}", _current_position, _config.control_key_min(), _config.control_key_max());
	return _current_position == _config.control_key_position();
}

namespace {
	void update_position(int& position, const configuration& config, bool reset, int delta) {
		spdlog::debug("position={}, delta={}, reset={}", position, delta, reset);
		if (reset) {
			position = 0;
		} else {
			position = std::clamp(position + delta, config.control_key_min(), config.control_key_max());
		}
		spdlog::debug("new_position={}, min={}, max={}", position, config.control_key_min(), config.control_key_max());
	}
} // namespace
} // namespace turenar::koatc

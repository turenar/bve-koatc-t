#pragma once

namespace turenar::koatc {
class configuration;

class control_key_manager {
public:
	control_key_manager(const configuration&);

	void turn_right();
	void turn_left();
	[[nodiscard]] bool active() const;

private:
	const configuration& _config;
	int _current_position;
};
} // namespace turenar::koatc

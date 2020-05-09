#pragma once

namespace turenar::koatc {
namespace wrapper {
	class atc_output;
}
class configuration;

class control_key_manager {
public:
	explicit control_key_manager(const configuration&);

	void turn_right();
	void turn_left();
	[[nodiscard]] bool active() const;
	void output(wrapper::atc_output) const;

private:
	const configuration& _config;
	int _current_position;
};
} // namespace turenar::koatc

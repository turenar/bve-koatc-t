#include "koatc/wrapper/atc_output.hxx"

namespace turenar::koatc::wrapper {
atc_output::atc_output(int* panels, int* sound) : _panels(panels), _sounds(sound) {
	set_sound(sound_id::bell, bve::ats::sound_control::play_continue);
	set_sound(sound_id::buzzer, bve::ats::sound_control::play_continue);
}
void atc_output::set_panel(panel_id id, int value) {
	int num = static_cast<int>(id);
	assert(0 <= num && num < panel_count);
	_panels[num] = value;
}
void atc_output::set_sound(sound_id id, bve::ats::sound_control control) {
	int num = static_cast<int>(id);
	assert(0 <= num && num <= sound_count);
	_sounds[num] = static_cast<int>(control);
}
} // namespace turenar::koatc::wrapper

#pragma once

#include <cassert>
#include "ats/sound_control.hxx"
#include "koatc/panel_id.hxx"
#include "koatc/sound_id.hxx"

namespace turenar::koatc::wrapper {
class atc_output {
private:
	constexpr static int panel_count = 256;
	constexpr static int sound_count = 256;

public:
	atc_output(int* panels, int* sound) : _panels(panels), _sounds(sound) {}

	void set_panel(panel_id id, int value) {
		int num = static_cast<int>(id);
		assert(0 <= num && num < panel_count);
		_panels[num] = value;
	}
	void set_sound(sound_id id, bve::ats::sound_control control) {
		int num = static_cast<int>(id);
		assert(0 <= num && num <= sound_count);
		_sounds[num] = static_cast<int>(control);
	}

private:
	int* const _panels;
	int* const _sounds;
};
} // namespace turenar::koatc::wrapper

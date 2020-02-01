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
	atc_output(int* panels, int* sound);

	void set_panel(panel_id id, int value);
	void set_sound(sound_id id, bve::ats::sound_control control);

private:
	int* const _panels;
	int* const _sounds;
};
} // namespace turenar::koatc::wrapper

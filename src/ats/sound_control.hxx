#pragma once

#include "ats/atsplugin.hxx"

namespace bve::ats {
enum class sound_control {
	stop = ATS_SOUND_STOP,
	play = ATS_SOUND_PLAY,
	play_loop = ATS_SOUND_PLAYLOOPING,
	play_continue = ATS_SOUND_CONTINUE,
};
}

#pragma once

#include "koatc/sound_id.hxx"
#include "koatc/wrapper/atc_output.hxx"

namespace turenar::koatc {
class sound_player {
private:
	constexpr static int invalid_time = -1;

public:
	explicit sound_player(sound_id id) : _id(id) {}

	void start(int time, int duration) {
		_end = time + duration;
	}
	void start() {
		_end = std::numeric_limits<int>::max();
	}
	void stop() {
		if (_end != invalid_time) {
			_end = 0;
		}
	}
	void output(wrapper::atc_output out, int time) {
		if (_end == invalid_time) {
			// do nothing
		} else if (time < _end) {
			out.set_sound(_id, bve::ats::sound_control::play_loop);
		} else {
			out.set_sound(_id, bve::ats::sound_control::stop);
			_end = invalid_time;
		}
	}
	void reset() {
		_end = invalid_time;
	}

private:
	const sound_id _id;
	int _end = invalid_time;
};
} // namespace turenar::koatc

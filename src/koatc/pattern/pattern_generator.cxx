#include "koatc/configuration.hxx"
#include "koatc/pattern/init_requirements.hxx"
#include "koatc/pattern/pattern_generator.hxx"

namespace turenar::koatc::pattern {
namespace {
	double deceleration(const configuration& config, bool use_emergency) {
		return use_emergency ? config.deceleration_emergency() : config.deceleration();
	}
} // namespace

// km/h/s -> km/1000 h^2
pattern_generator::pattern_generator(const init_requirements& req, bool use_emergency)
		: _config(req.config), _vehicle_state(req.state), _curve(deceleration(req.config, use_emergency) * 3.6),
		  _use_emergency(use_emergency) {}
void pattern_generator::clear() {
	_curve.clear();
	_flat.clear();
}
void pattern_generator::set_flat_speed(int speed) {
	_curve.clear();
	_flat.generate_pattern(speed);
}
void pattern_generator::set_target_speed(double location, int speed) {
	set_target_speed(location, speed, speed);
}
void pattern_generator::set_target_speed(double location, int flat_speed, int bottom) {
	_curve.generate_pattern(location, flat_speed, bottom);
	_flat.generate_pattern(flat_speed);
}
void pattern_generator::tick() {
	update_pattern();
	update_handle();
}
void pattern_generator::update_pattern() {
	if (_curve.active(_vehicle_state.location)) {
		_current_limit = _curve.limit_of(_vehicle_state.location);
		_current_bottom = _curve.bottom();
	} else {
		_current_limit = _flat.limit_of(_vehicle_state.location);
		_current_bottom = _flat.bottom();
	}
}
void pattern_generator::update_handle() {
	if (_use_emergency) {
		update_handle_emergency();
	} else {
		update_handle_normal();
	}
}
void pattern_generator::update_handle_emergency() {
	double over_speed = _vehicle_state.speed - _current_limit;
	if (over_speed > _config.pattern_offset()) {
		_handle = handle_command::emergency();
	} else {
		_handle = handle_command::neutral();
	}
}
void pattern_generator::update_handle_normal() {
	double over_speed = _vehicle_state.speed - _current_limit;
	if (over_speed > _config.pattern_offset_emergency()) {
		_handle = handle_command::emergency();
	} else if (over_speed > _config.pattern_offset_full()) {
		_handle = handle_command::full_brake();
	} else if (over_speed > _config.pattern_offset()) {
		_handle = handle_command::half_brake();
	} else {
		_handle = handle_command::neutral();
	}
}
} // namespace turenar::koatc::pattern

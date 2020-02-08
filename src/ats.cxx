#include <windows.h>
#include <filesystem>
#include <memory>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>
#include "ats/atsplugin.hxx"
#include "ats/sound_control.hxx"
#include "ats/vehicle_spec.hxx"
#include "koatc/atc_manager.hxx"
#include "koatc/configuration.hxx"

namespace {
bve::ats::vehicle_spec spec;
turenar::koatc::configuration config;
std::unique_ptr<turenar::koatc::atc_manager> manager;

std::filesystem::path get_module_path(HINSTANCE instance);
BOOL initialize_logger(const std::filesystem::path&);
void shutdown_logger();
} // namespace

extern "C" {
BOOL APIENTRY DllMain(HINSTANCE instance, DWORD reason, LPVOID) {
	BOOL result = TRUE;
	switch (reason) {
	case DLL_PROCESS_ATTACH: {
		auto&& path = get_module_path(instance);
		result &= initialize_logger(path);
		std::filesystem::path config_path{path};
		config_path.replace_extension(".ini");
		config = turenar::koatc::configuration{config_path};
	} break;
	case DLL_PROCESS_DETACH:
		shutdown_logger();
		break;
	default:; // ignore
	}
	return result;
}

ATS_API void WINAPI Load(void) {}

ATS_API void WINAPI Dispose(void) {}

ATS_API int WINAPI GetPluginVersion(void) {
	return ATS_VERSION;
}

ATS_API void WINAPI SetVehicleSpec(ATS_VEHICLESPEC sp) {
	spec = sp;
	manager = std::make_unique<turenar::koatc::atc_manager>(config, spec);
	spdlog::info("{}", config);
}

ATS_API void WINAPI Initialize(int) {}

ATS_API ATS_HANDLES WINAPI Elapse(ATS_VEHICLESTATE state, int* panel, int* sound) {
	return manager->tick(state, turenar::koatc::wrapper::atc_output{panel, sound});
}

ATS_API void WINAPI SetPower(int power) {
	manager->power(bve::ats::notch_position{power});
}

ATS_API void WINAPI SetBrake(int brake) {
	manager->brake(bve::ats::notch_position{brake});
}

ATS_API void WINAPI SetReverser(int reverser) {
	manager->reverser(static_cast<bve::ats::reverser_position>(reverser));
}

ATS_API void WINAPI KeyDown(int) {}

ATS_API void WINAPI KeyUp(int) {}

ATS_API void WINAPI HornBlow(int) {}

ATS_API void WINAPI DoorOpen(void) {}

ATS_API void WINAPI DoorClose(void) {}

ATS_API void WINAPI SetSignal(int open_section) {
	manager->set_open_section(open_section);
}

ATS_API void WINAPI SetBeaconData(ATS_BEACONDATA beacon) {
	manager->put_beacon(bve::ats::beacon{beacon});
}
}

namespace {
std::filesystem::path get_module_path(HINSTANCE instance) {
	TCHAR module_path[_MAX_PATH];
	DWORD result = GetModuleFileName(instance, module_path, std::size(module_path));
	if (result == 0) {
		return {};
	}
	return {module_path};
}
BOOL initialize_logger(const std::filesystem::path& module_path) {
	std::filesystem::path log_path{module_path};
	log_path.replace_extension(".log");

	auto&& logger = spdlog::basic_logger_st("KO-ATC", log_path.string(), true);
	spdlog::set_default_logger(std::move(logger));
	spdlog::set_level(spdlog::level::trace);
	return TRUE;
}
void shutdown_logger() {
	spdlog::shutdown();
}
} // namespace

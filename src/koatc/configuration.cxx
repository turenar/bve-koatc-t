#include <windows.h>
#include <tchar.h>
#include <spdlog/spdlog.h>
#include "koatc/configuration.hxx"
#include "koatc/logger/wchar_wrapper.hxx"

namespace turenar::koatc {
namespace {
	constexpr const TCHAR* section_name = _T("KO-ATC-T");

	template <typename Result, typename Converter>
	void parse(Result& target, LPCTSTR section, LPCTSTR key, LPCTSTR file, Converter);

	void parse_double(double& target, LPCTSTR section, LPCTSTR key, LPCTSTR file);
	void parse_int(int& target, LPCTSTR section, LPCTSTR key, LPCTSTR file);
} // namespace

configuration::configuration() = default;
configuration::configuration(const std::filesystem::path& path) {
	auto file = path.c_str();
	parse_double(_deceleration, section_name, _T("Deceleration"), file);
	parse_double(_deceleration_emergency, section_name, _T("DecelerationEmergency"), file);
	parse_int(_pattern_offset, section_name, _T("PatternOffsetNormal"), file);
	parse_int(_pattern_offset_full, section_name, _T("PatternOffsetFull"), file);
	parse_int(_pattern_offset_emergency, section_name, _T("PatternOffsetEmergency"), file);

	parse_int(_pattern_approaching, section_name, _T("PatternApproaching"), file);
	parse_int(_bell_threshold, section_name, _T("BellThreshold"), file);
	parse_int(_section_margin, section_name, _T("SectionMargin"), file);
	parse_int(_emergency_time, section_name, _T("EmergencyTime"), file);

	parse_int(_control_key_position, section_name, _T("ControlKeyPosition"), file);
	parse_int(_control_key_max, section_name, _T("ControlKeyMax"), file);
	parse_int(_control_key_min, section_name, _T("ControlKeyMin"), file);
	parse_int(_power_on_time, section_name, _T("PowerOnTime"), file);
}

namespace {
	template <typename Result, typename Converter>
	void parse(Result& target, LPCTSTR section, LPCTSTR key, LPCTSTR file, const char* type, Converter converter) {
		TCHAR buf[256];
		DWORD len = GetPrivateProfileString(section, key, nullptr, buf, std::size(buf), file);
		if (len) {
			LPTSTR endptr;
			auto result = static_cast<Result>(converter(buf, &endptr));
			if (*endptr == _T('\0')) {
				target = result;
			} else {
				spdlog::warn(
						"invalid configuration for {}.{} (not {}): {}",
						logger::wchar_wrapper{section},
						logger::wchar_wrapper{key},
						type,
						logger::wchar_wrapper{buf});
			}
		}
	}

	void parse_double(double& target, LPCTSTR section, LPCTSTR key, LPCTSTR file) {
		parse(target, section, key, file, "number", &std::wcstod);
	}
	void parse_int(int& target, LPCTSTR section, LPCTSTR key, LPCTSTR file) {
		parse(target, section, key, file, "integer", [](LPCTSTR str, LPTSTR* endptr) {
			return std::wcstol(str, endptr, 10);
		});
	}
} // namespace
} // namespace turenar::koatc

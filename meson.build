project('bve_koatc', 'cpp',
	default_options: [
		'cpp_std=c++17',
		'warning_level=3',
		'default_library=static',
		'b_ndebug=if-release',
	],
	license: 'MIT',
	meson_version: '>=0.46',
	version: '0.1')


cpp_compiler = meson.get_compiler('cpp')

add_project_arguments('-DUNICODE', language: 'cpp')
if cpp_compiler.get_id() == 'msvc'
	add_project_arguments('/utf-8', language: 'cpp')
endif
if get_option('buildtype') == 'debug'
	add_project_arguments('-DSPDLOG_ACTIVE_LEVEL=0', language: 'cpp')
endif

if cpp_compiler.get_id() != 'msvc'
	cppflags = []
	ldflags = [
		'-static',
		'-mwindows',
		'-Wl,--add-stdcall-alias',
		'-Wl,--enable-stdcall-fixup',
	]

	if get_option('warning_level') == '3'
		cppflags += [
			'-Wconversion',
			'-Wctor-dtor-privacy',
			'-Wdelete-non-virtual-dtor',
			'-Wdisabled-optimization',
			'-Wfloat-equal',
			'-Wlogical-op',
			'-Wnoexcept',
			'-Wold-style-cast',
			'-Woverloaded-virtual',
			'-Wsuggest-final-methods',
			'-Wsuggest-final-types',
			'-Wsuggest-override',
			'-Wswitch-default',
			'-Wundef',
		]
	endif

	add_project_arguments(cpp_compiler.get_supported_arguments(cppflags), language: 'cpp')

	add_project_link_arguments(
		cpp_compiler.get_supported_link_arguments(ldflags),
		language: 'cpp')
endif

# ignore warnings in spdlog
spdlog_orig_dep = dependency('spdlog',
	fallback: ['spdlog', 'spdlog_dep'],
	default_options: ['enable_tests=false', 'enable_examples=false'])
spdlog_dep = spdlog_orig_dep.as_system()

subdir('src')

ats = shared_library('Ats', src_files,
	vs_module_defs: 'src/ats.def',
	cpp_args: ['-DATS_EXPORTS'],
	dependencies: [spdlog_dep],
	name_prefix: '',
	include_directories: include_directories('include', 'src'))

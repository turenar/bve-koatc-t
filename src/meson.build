subdir('ats')
subdir('koatc')

vcs_target = vcs_tag(
	input: 'version.hxx.in',
	output: 'version.hxx')

src_files = [vcs_target, ats_files, koatc_files, files([
	'ats.cxx',
])]

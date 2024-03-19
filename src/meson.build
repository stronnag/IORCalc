
vapi_dir = meson.current_source_dir() / 'vapi'

add_project_arguments(['--vapidir', vapi_dir], language: 'vala')
add_project_arguments(['--pkg', 'ior'], language: 'vala')

host_os = host_machine.system()

libsrc = files('ioralloc.c', 'gluefuncs.c', 'calc_hull.c', 'calc_rig.c', 'calc_rtg.c',
	       'print.c', 'calc_prop.c', 'calc_rmc.c', 'calc_rig_extra.c',
	       'cert.c', 'iorutil.c', 'iorconst.c', 'report.c', 'plotdata.c')

iorsrcs = files('iorcalc.vala', 'iorwindow.vala', 'iorprint.vala', 'iorcertwin.vala',
		'iorsettings.vala', 'jsonio.vala', 'plotwindow.vala', 'iormisc.vala')

dynalib = both_libraries('iorcalc',
                         libsrc,
			 c_args: '-D_GNU_SOURCE',
                         install_rpath: get_option('prefix') + '/lib',
			 dependencies: [meson.get_compiler('c').find_library('m')],
			 install : true)

if host_os == 'windows'
  windows = import('windows')
  iorsrcs += windows.compile_resources('winres.rc')
endif

vopts += '--define=OS_' + host_os

install_headers('ior.h', 'iorext.h')
install_data('vapi/ior.vapi', install_dir : get_option('datadir') / 'vala' / 'vapi')

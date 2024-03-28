
vapi_dir = meson.current_source_dir() / 'vapi'

add_project_arguments(['--vapidir', vapi_dir], language: 'vala')
add_project_arguments(['--pkg', 'ior'], language: 'vala')

host_os = host_machine.system()

libsrc = files('ioralloc.c', 'gluefuncs.c', 'calc_hull.c', 'calc_rig.c', 'calc_rtg.c',
	       'print.c', 'calc_prop.c', 'calc_rmc.c', 'calc_rig_extra.c',
	       'cert.c', 'iorutil.c', 'iorconst.c', 'report.c', 'plotdata.c')

iorsrcs = files('iorcalc.vala', 'iorwindow.vala', 'iorprint.vala', 'iorcertwin.vala',
		'iorsettings.vala', 'plotwindow.vala', 'iormisc.vala')

libargs = {
  'sources': [libsrc, 'jsonio.vala'],
  'c_args': '-D_GNU_SOURCE',
  'dependencies': [    dependency('gio-2.0'),
                       dependency('json-glib-1.0'),
                       meson.get_compiler('c').find_library('m')],
  'install': true
}

if meson.version().version_compare('>= 1.2.0')
  libargs += { 'install_dir': [ get_option('prefix') / 'lib', true, true] }
else
  libargs += { 'install_dir': get_option('prefix') / 'lib', }
  # It will be necessary to install the *.h and *.vapi manually if required.
endif

if dynalib
  iorlib = both_libraries('iorcalc', kwargs: libargs)
else
  iorlib = static_library('iorcalc', kwargs: libargs)
endif

if host_os == 'windows'
  windows = import('windows')
  iorsrcs += windows.compile_resources('winres.rc')
endif

vopts += '--define=OS_' + host_os

install_headers('ior.h', 'iorext.h')
install_data('vapi/ior.vapi', install_dir : get_option('datadir') / 'vala' / 'vapi')

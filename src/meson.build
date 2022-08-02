
vapi_dir = meson.current_source_dir() / 'vapi'

add_project_arguments(['--vapidir', vapi_dir], language: 'vala')
add_project_arguments(['--pkg', 'ior'], language: 'vala')

iorsrcs = files('iorcalc.vala', 'iorprint.vala', 'gluefuncs.c', 'jsonio.vala', 'calc_hull.c',
		'calc_rig.c', 'calc_rtg.c', 'print.c', 'calc_prop.c', 'calc_rmc.c',
		'cert.c', 'iorutil.c', 'iorconst.c', 'report.c')

if host_machine.system() == 'windows'
  windows = import('windows')
  iorsrcs += windows.compile_resources('winres.rc')
endif

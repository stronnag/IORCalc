project('iorcalc', 'vala', 'c', version : '1.3.2')

show_deprecated = get_option('deprecations')
if show_deprecated  == false
  add_project_arguments('-Wno-deprecated-declarations', language : 'c')
  add_project_arguments('--enable-deprecated', language : 'vala')
endif
force_s = get_option('force-static')
force_d = get_option('force-dynamic')

dynalib = false
if force_d
  dynalib = true
elif force_s
  dynalib = false
else
  dynalib = build_machine.system() == 'linux'
endif

conf_data = configuration_data()
conf_data.set('version', meson.project_version())

app = 'iorcalc'
gnome = import('gnome')

wresources = gnome.compile_resources(
  'i-resources',
  join_paths('data', app + '.gresource.xml'),
  source_dir: 'data',
  c_name: 'i'
)

deps = []
vopts = []

configure_file(
  input : 'src/iorcalc-config.h.in',
  output : 'iorcalc-config.h',
  configuration : conf_data
)
configuration_inc = include_directories('.','src')

subdir('valaopts')
subdir('meson')
subdir('data')
subdir('src')
subdir('samples')

gdeps = [
    dependency('gtk4'),
    dependency('gio-2.0'),
    dependency('json-glib-1.0'),
    dependency('pangocairo'),
]


exargs = {
  'sources': [wresources, iorsrcs],
  'dependencies': [ gdeps, deps,
    meson.get_compiler('vala').find_library('posix'),
    meson.get_compiler('c').find_library('m'),
  ],
  'include_directories': configuration_inc,
  'c_args': [ '-D_GNU_SOURCE', vcargs],
  'vala_args': vopts,
  'link_with': iorlib,
  'install': true,
  'gui_app': true,
}

if dynalib
    exargs += { 'install_rpath': '$ORIGIN/../lib',}
endif

executable(meson.project_name(), kwargs: exargs)

if host_machine.system() == 'windows'
  exargs += { 'build_by_default': false, 'install': false, 'gui_app': false,}
  executable('iorcalc-console', kwargs: exargs)
elif host_machine.system() == 'darwin'
  gnome.post_install(gtk_update_icon_cache: true,)
else
  gnome.post_install(gtk_update_icon_cache: true, update_desktop_database: true)
endif

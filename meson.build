project('iorcalc', 'vala', 'c', version : '1.3.3')

show_deprecated = get_option('deprecations')
if show_deprecated  == false
  add_project_arguments('-Wno-deprecated-declarations', language : 'c')
  add_project_arguments('--enable-deprecated', language : 'vala')
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

executable(
  meson.project_name(),
  wresources,
  iorsrcs,
  dependencies: [
    dependency('gtk4'),
    dependency('gio-2.0'),
    dependency('json-glib-1.0'),
    dependency('pangocairo'),
    deps,
    meson.get_compiler('vala').find_library('posix'),
    meson.get_compiler('c').find_library('m'),
  ],
  include_directories : configuration_inc,
  c_args: [ '-D_GNU_SOURCE', vcargs],
  vala_args : vopts,
  link_with : dynalib,
  install_rpath: get_option('prefix') + '/lib',
  install: true,
  gui_app: true,
)

if host_machine.system() == 'windows'
executable(
  'iorcalc-console',
  wresources,
  iorsrcs,
  dependencies: [
    dependency('gtk4'),
    dependency('gio-2.0'),
    dependency('json-glib-1.0'),
    dependency('pangocairo'),
    deps,
    meson.get_compiler('vala').find_library('posix'),
    meson.get_compiler('c').find_library('m'),
  ],
  include_directories : configuration_inc,
  c_args: [ '-D_GNU_SOURCE', vcargs],
  vala_args : vopts,
  link_with : staticlib,
  build_by_default: false,
  install: false,
  gui_app: false,
)
elif host_machine.system() == 'darwin'
  gnome.post_install(
    gtk_update_icon_cache: true,
  )
else
  gnome.post_install(
    gtk_update_icon_cache: true,
    update_desktop_database: true
  )
endif

# vala_args : dopts,

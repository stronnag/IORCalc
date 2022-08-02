project('iorcalc', 'vala', 'c', version : '0.7.0')

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

configure_file(
  input : 'src/iorcalc-config.h.in',
  output : 'iorcalc-config.h',
  configuration : conf_data
)
configuration_inc = include_directories('build','src')

subdir('src')

executable(
  meson.project_name(),
  wresources,
  iorsrcs,
  dependencies: [
    dependency('gtk+-3.0'),
    dependency('gio-2.0'),
    dependency('json-glib-1.0'),
    dependency('pangocairo'),
    deps,
    meson.get_compiler('vala').find_library('posix'),
    meson.get_compiler('c').find_library('m'),
  ],
  include_directories : configuration_inc,
  c_args: '-D_GNU_SOURCE',
  install: true,
  gui_app: true,
)

subdir('samples')
subdir('data')

#gnome.post_install(
#  glib_compile_schemas: true,
#  gtk_update_icon_cache: true,
#  update_desktop_database: true
#)
meson.add_install_script('meson/post_install.sh')

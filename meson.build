project(
    'com.github.braycarlson.forknife',
    'c',
    version: '0.1',
    license: 'MIT'
)

giodep = dependency('gio-2.0', method: 'pkg-config')
glibdep = dependency('glib-2.0', method: 'pkg-config')
gtkdep = dependency('gtk+-3.0', method: 'pkg-config')

gnome = import('gnome')

resources = gnome.compile_resources(
    'resources',
    'resources/resource.gresource.xml',
    gresource_bundle: true,
    install: true,
    install_dir: meson.current_build_dir(),
    source_dir: meson.current_build_dir()
)

sources = ['src/main.c', resources]

executable(
    meson.project_name(),
    sources,
    dependencies: [giodep, gtkdep],
    gui_app: true
)

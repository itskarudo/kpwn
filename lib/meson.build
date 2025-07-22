lib_sources = files(
  'src/bytes.c',
  'src/cyclic.c',
  'src/encoding.c',
  'src/io.c',
  'src/kpwn.c',
  'src/log.c',
  'src/utils.c',
)

libkpwn = static_library('kpwn',
  lib_sources,
  include_directories: include_directories('include'),
  install: true,
  override_options: ['c_std=gnu18'],
)

libkpwn_dep = declare_dependency(
  include_directories: include_directories('include'),
  link_with: libkpwn,
  version: meson.project_version()
)

lib_headers = files(
  'include/bytes.h',
  'include/context.h',
  'include/cyclic.h',
  'include/encoding.h',
  'include/io.h',
  'include/kpwn.h',
  'include/log.h',
  'include/utils.h',
)

install_headers(lib_headers, subdir: 'kpwn')

pkg = import('pkgconfig')
pkg.generate(libkpwn,
  name: 'kpwn',
  description: 'The kernel exploitation toolkit',
  version: meson.project_version()
)

meson.override_dependency('libkpwn', libkpwn_dep)

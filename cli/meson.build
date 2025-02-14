bin_sources = files(
  'src/main.cpp',
  'src/cmds/init.cpp',
)

argparse_dep = dependency('argparse', 
  fallback: ['argparse', 'argparse_dep'],
)

kpwn_cli = executable('kpwn',
  bin_sources,
  dependencies: [argparse_dep, libkpwn_dep],
  include_directories: include_directories('include'),
  install: true,
  override_options: ['cpp_std=c++20'],
)

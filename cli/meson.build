bin_sources = files(
  'src/main.cpp',
  'src/cmds/init.cpp',
)

prefix = get_option('prefix')
conf_data = configuration_data()
conf_data.set('KPWN_PREFIX', '"' + prefix + '"')

configure_file(output : 'config.h',
               configuration : conf_data)

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

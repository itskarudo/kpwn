#include "cmds.h"
#include "config.h"
#include <filesystem>
#include <fstream>
#include <kpwn.h>

constexpr const char *main_template = R"(#include <kpwn/kpwn.h>

int main(void) {
  log_info("Hello, world!\n");
}
)";

constexpr const char *meson_template = R"(
project('exploit', 'c', version: '1.0.0')

src_files = files(
  'src/main.c'
)

kpwn_prefix = ')" KPWN_PREFIX R"('

kpwn_lib_dir = kpwn_prefix + '/lib'
kpwn_header_dir = kpwn_prefix + '/include'

kpwn_dep = declare_dependency(
    link_args : ['-L' + kpwn_lib_dir, '-lkpwn'],
    include_directories : include_directories(kpwn_header_dir))

executable('exploit',
  src_files,
  dependencies: [kpwn_dep],
  link_args: ['-static'],
)
)";

constexpr const char *compress_template = R"(#!/bin/bash

if [ ! -d "./initramfs" ]; then
  echo "./initramfs/ directory does not exist."
  exit 1
fi

ninja -C ../build || exit 1

cp "../build/exploit" "./initramfs/"
cd ./initramfs

find . -print0 \
| cpio --null -ov --format=newc \
| gzip -9 > initramfs.cpio.gz

mv ./initramfs.cpio.gz ../

)";

namespace cmds {
void init(argparse::ArgumentParser const &cmd_options) {

  log_info("Initializing the project..\n\n");

  auto dir = cmd_options.get<std::string>("directory");

  auto path =
      std::filesystem::weakly_canonical(std::filesystem::current_path() / dir);

  try {

    if (!std::filesystem::is_directory(path))
      std::filesystem::create_directory(path);

    std::ofstream meson_build(path / "meson.build");
    meson_build << meson_template;
    meson_build.close();

    log_info("Created meson.build template\n");

    std::filesystem::create_directory(path / "src");

    std::ofstream main_c(path / "src/main.c");
    main_c << main_template;
    main_c.close();

    log_info("Created src/main.c template\n");

    std::filesystem::create_directory(path / "chal");

    std::ofstream compress_sh(path / "chal/compress.sh");
    compress_sh << compress_template;
    compress_sh.close();
    std::filesystem::permissions(path / "chal/compress.sh",
                                 std::filesystem::perms::owner_exec,
                                 std::filesystem::perm_options::add);

    log_info("Created chal/compress.sh template\n");

    std::putchar('\n');
    log_success("Initialized the project, happy hacking!\n\n");

  } catch (std::exception &err) {
    log_error("%s\n", err.what());
  }
}

} // namespace cmds

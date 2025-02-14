#include "cmds.h"
#include <filesystem>
#include <format>
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

executable('exploit',
  src_files,
  dependencies: [dependency('kpwn')],
  link_args: ['-static']
)
)";

constexpr const char *compress_template = R"(#!/bin/bash

if [ ! -d "./initramfs" ]; then
  echo "./initramfs/ directory does not exist."
  exit 1
fi

make -C ../build || exit 1

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

    // TODO: support custom meson options and different build systems
    std::system(
        std::format("meson setup {}/build {}", path.string(), path.string())
            .c_str());
    std::system(std::format("ninja -C {}/build", path.string()).c_str());

    std::putchar('\n');
    log_success("Initialized the project, happy hacking!\n\n");

  } catch (std::exception &err) {
    log_error("%s\n", err.what());
  }
}

} // namespace cmds

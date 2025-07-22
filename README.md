# kpwn - The kernel exploitation toolkit

Kpwn is an exploit development library for C, it is highly focused on kernel exploitation where C is most often the only option you have, since you're locked in an isolated minimal environemnt (qemu with busybox most often), it gives you a bunch of useful types, methods and objects that you would miss when pwntools isn't an option.

## Example usage

```c
#include <kpwn/kpwn.h>
int main(void) {

    au(io_t) io = io_new("/dev/ctf", O_RDWR);

    io_write(io, b_mul(b("A"), 0x20));
    io_read(io, 0x20);
    uint64_t stack_leak = u64(io_read(io, 8));

    log_success("Leaked a stack address!\n");
    log_info("stack @ %p\n", stack_leak);

}
```

## Installation

Kpwn is not recommended to be installed at the default prefix (/usr) since that can cause conflicts between the system's libc and musl.

```bash
# Clone the repo
git clone https://github.com/itskarudo/kpwn.git
cd kpwn

# Build the library
export CC=musl-gcc # using musl is optional but recommended
meson setup build --prefix ~/.local
cd build && ninja

# Install
ninja install
```

## Quickstart

Kpwn comes with a CLI tool that automates creating a project that uses the libkpwn library.

```bash
kpwn init project_name
```

For a complete list of the provided functions and types, check the [lib/include directory](https://github.com/itskarudo/kpwn/tree/main/lib/include).

### `io_t`

The `io_t` type is a wrapper around a file descriptor that provides many helpful methods.

```c
#include <kpwn/kpwn.h>

int main(void) {

    // create new io_t object
    io_t io = io_new("/dev/chal", O_RDWR);

    // read 16 bytes
    io_read(io, 16);

    // raed until bytes are found
    io_readuntil(io, b("skibidi"));

    // read a line, equivalent to `io_readuntil(io, b("\n"))`
    io_readline();

    // write bytes
    io_write(io, b("something something"));

    // call ioctl with given arguments
    io_ioctl(io, ...);

    // closes the backing file descriptor
    io_close(io);

    // equivalent to `io_close(io)`
    io_destroy(io);

}
```

### `bytes_t`

The `bytes_t` type represents a sequence of bytes.

```c
#include <kpwn/kpwn.h>

int main(void) {

    // create empty 16 bytes bytes_t object
    bytes_t bytes = b_new(16);

    // create bytes_t object from uint8_t* and size
    bytes = b_new_v("ABCD", 4);

    // shorthand for `b_new_v(str, sizeof(str)-1)`
    bytes = b("ABCD");

    // return a read only slice (analog to std::string_view)
    b_slice(bytes, 1, -2);

    // compare 2 bytes_t objects
    b_cmp(bytes, b("DEFG"));

    // return string representation of `bytes`
    char* str = b_str(bytes);

    // append to bytes_t object
    b_append(&bytes, b("DEFG"));

    // free the backing memory of a bytes_t object
    b_destroy(bytes);

}
```

### Logging

the library supports multiple levels of logging that supports printf-style format strings.

```c
#include <kpwn/kpwn.h>

int main(void) {

    log_error("Failed to open file: %s", "data.txt");
    log_warning("Low disk space: %d%% left", 5);
    log_info("Server started on port %d", 8080);
    log_success("Data saved successfully");
    log_debug("Value of x is %d", 42);

}
```

### `context`

This is the global context used within kpwn, it's a singleton of type `context_t`.

```c
#include <kpwn/kpwn.h>

int main(void) {

    // set the alphabet used when calling cyclic(n)
    context.cyclic_alphabet = b("ABCDEFGHIJKLMNOPQRSTUVWXYZ");

    // set the sequence size of the debrujin sequence produced by cyclic(n)
    context.cyclic_size = 2;

    // set the log level used with log_* functions
    context.log_level = LOG_DEBUG;

}
```

### `au(type)`

the `au(type)` macro allows calling predefined destructor functions when a variable goes out of scope, for `io_t` and `bytes_t` these are `io_destroy` and `b_destroy` respectively.

**NOTE**: never use `au(bytes_t)` with slices as it can lead to a double free.

```c
#include <kpwn/kpwn.h>

int main(void) {

    {
        au(io_t) io = io_new("/dev/chal", O_RDWR);
        // `io_destroy(io)` will get called automatically here.
    }

}
```

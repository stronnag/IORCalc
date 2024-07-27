# Developer Examples

## Introduction

Just in case one feels the urge to become a retro IOR certificate authority, here's some examples of how to build your own IOR program based on this repository.

## Usage

In addition to the files here, you either need to have installed the project, so the iorcalc library, header files and 'vapi' (vala definitions) are available under `$prefix`, or to copy the requred files to your build directory.

If the files are installed to a local directory, rather than somewhere on the system search path, the Makefile uses `prefix`.

```
make prefix=~/.local
#
make prefix=/usr/local
```

The examples are:

* `vconv` : (Vala source). Converts a JSON data file into a binary data file (which may be used by `iorcalc`).
* `vprint` : (Vala source). Takes a binary data file and generates a single page (132 columns) certificate file.
* `cprint` : (C source). Takes a binary data file and generates a single page (132 columns) certificate file.

`vprint` and `cprint` do pretty much the same thing.

### FreeBSD

Use `gmake` rather than `make`.

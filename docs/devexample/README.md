# Developer Examples

## Introduction

Just in case one feels the urge to become a retro IOR certificate authority, here's some examples of how to build your own IOR program based on this repository.

## Usage

In addition to the files here, you either need from the main source (`src`) directory:

* `ior.h`
* `iorext.h`
* `vapi/ior.vapi` (into a local `vapi/` directory)
* `jsonio.vala`

or set some variables for the Makefile:

* `INCDIR` : Where to find the `ior*.h` include files
* `SRCDIR` : Where to find `jsonio.vala` and the `vapi` directory

And in either case, set the `LIBDIR` variable to where `libiorcalc.a` may be found, if not in `$HOME/.local/lib`, e..g:

```
make LIBDIR=/usr/local/lib
```

or, assuming the `iorcalc` directory was `~/Projects/iorcalc`, without any copying:

```
make LIBDIR=~/Projects/iorcalc/build/src INCDIR=~/Projects/iorcalc/src SRCDIR=~/Projects/iorcalc/src

```

The examples are:

* `vconv` : (Vala source). Converts a JSON data file into a binary data file (which may be used by `iorcalc`).
* `vprint` : (Vala source). Takes a binary data file and generates a single page (132 columns) certificate file.
* `cprint` : (C source). Takes a binary data file and generates a single page (132 columns) certificate file.

`vprint` and `cprint` do pretty much the same thing.

### FreeBSD

Use `gmake` rather than `make`.

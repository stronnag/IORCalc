# IORCalc - Calculate IOR Ratings

## Features

*Calculate IOR ratings like it's 1988.*

* Calculate IOR Ratings based in the 1988 IOR Rule
* Input data as a JSON file
* Graphical data editor
* Generate IOR certificate

This is based on a program I wrote in 1984 and maintained until 1988 based on the ORC Red Book and amendments. It originally ran on a Sinclair QL QDOS operating system.

It has been recently modernised to contemporary C standards and a GTK3 UI added to replace the legacy QL UI.

It is largely complete, missing only some esoteric rig options (Gaff rig etc), and believed to be accurate (when last compared to an official certificate in 1986, it was in complete agreement). The later changes 1987-88 are unverified.

## Building

Requirements:

* GTK 4 and json-glib development files (gtk3 for legacy branch).
* Vala and C compilers
* Meson / Ninja build system

### Linux, FreeBSD

* Common GTK packages (meson (and ninja), vala, gtk(3 or 4), json-glib); the `master` branch is `gtk4`, the `gtk3` branch is (pause ...) `gtk3`.

* Easily built from source
    ```
        # Once (setup)
        meson build --buildtype=release --strip --prefix=~/.local
        # Build and install to ~/.local/bin (add to PATH if necessary)
        meson install -C build
   ```
### Windows

* Can be built from source using Msys2.
    ```
        pacman -Syu
        pacman -S gtk4 vala meson ninja json-glib
        meson build --buildtype=release --strip
		meson install -C build
        ```
### MacOS

* Use Homebrew:
    ```
        # install requirements:
        brew install meson vala gtk4 json-glib
        # Once (setup)
        meson build --buildtype=release --strip --prefix=~/.local
        # Build and install to ~/.local/bin (add to PATH if necessary)
        meson install -C build
   ```
* If there are missing icons, it may be necessary to `brew install adwaita-icon-theme`.

* It seems that GTK printing for MacOS is only functional using the `master` (gtk4) branch.

### Legacy GTK+-3.0 branch

There is also a `gtk3` branch. This is functionally equivalent to the GTK4/`master` branch.
In the above build instructions, replace `gtk4` with the equivalent `gtk3` / `gtk+-3.0` package name.

## Binary Release Media

There is a Debian package (`*.deb`) and Windows Installer in the [release area](https://github.com/stronnag/IORCalc/releases). These are both built from the **gtk3** branch.

## Example files

Two example input files are provided (`samples` in the repo, installed in `share/doc/iorcalc`).

* summerwine.json : 1984 Humphreys one tonner
* manitou4.json : 1969 C&C 39

## User Guide

There is a [user guide](docs/)

## Miscellaneous

Licence : GPL v3 or later
(c) Jonathan Hudson 1984 - 2022

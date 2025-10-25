# Migrated to [codeberg.org](https://codeberg.org/stronnag/IORCalc)

## IORCalc - Calculate IOR Ratings

## Features

*Calculate IOR ratings like it's 1988.*

* Calculate IOR Ratings based in the 1988 IOR Rule
* Input data as a JSON file
* Graphical data editor
* Generate IOR certificate

This is based on a program I wrote in 1984 and maintained until 1988 based on the ORC Red Book and amendments. It originally ran on a Sinclair QL / QDOS operating system.

It has been recently modernised to contemporary C standards and a GTK UI added to replace the legacy QL TUI.

It is a complete IOR'88 implementation, including all rig options (even Appendix 7  gaff rig).

* The more esoteric rig options (ketch, schooner, yawl, gaff are untested as the author has no examples).
* The post 1986 (1987-88) updates are untested.

Otherwise, it is believed to be accurate; when last compared to official certificates in 1986, it was in complete agreement (with RORC Rating Office issued certificates).

## Binary Release Media

There is a Debian package (`*.deb`) and Windows Installer in the [release area](https://github.com/stronnag/IORCalc/releases).

[Build Instructions](#building) are provided for other platforms; tested on Arch Linux, FreeBSD and MacOS.

## Example files

Two example input files are provided (`samples` in the repo, installed in `share/doc/iorcalc`).

* manitou4.json : 1969 C&C 39
* summerwine.json : 1984 Humphreys one tonner

## User Guide

There is a [user guide](docs/)

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
        # specific package requirements:
        brew install meson vala gtk4 json-glib
        # Once (setup)
        meson build --buildtype=release --strip --prefix=~/.local
        # Build and install to ~/.local/bin (add to PATH if necessary)
        meson install -C build
   ```
* If there are missing icons, it may be necessary to `brew install adwaita-icon-theme`.

* It seems that GTK printing for MacOS is only functional using the `master` (gtk4) branch.

### Legacy GTK+-3.0 branch

There is a `gtk3` branch. This is less functional than the GTK4/`master` branch, and is no longer maintained. It does however compile and function for users of older Linux distributions. There is no need to use it on modern Linux, FreeBSD, MacOS or Windows.

In the above build instructions, replace `gtk4` with the equivalent `gtk3` / `gtk+-3.0` package name.

## Miscellaneous

Licence : GPL v3 or later
(c) Jonathan Hudson 1984 - 2022

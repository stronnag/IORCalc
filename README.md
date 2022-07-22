# IORCalc - Calculate IOR Ratings

## Features

* Calculate IOR Ratings based in the 1988 IOR Rule
* Input data as a JSON file
* Graphical data editor
* Generate IOR certificate

This is based on a program I wrote in 1984 and maintained until 1988 based on the ORC Red Book and amendments. It originally ran on a Sinclair QL QDOS operating system.

It has been recently modernised to contemporary C standards and a GTK3 UI added to replace the legacy QL UI.

It is largely complete, missing only some esoteric rig options (Gaff rig etc), and believed to be accurate (when last compared to an official certificate in 1986, it was in complete agreement). The later changes 1987-88 are unverified.

## Building

Requirements:

* GTK 3 and json-glib development files
* Vala and C compilers
* Meson / Ninja build system

### Linux, FreeBSD

* Common GTK packages (meson (and ninja), vala, gtk+3, json-glib)
* Easily built from source
    ```
        # Once (setup)
        meson build --buildtype=release --strip --prefix=~/.local
        # Build and install to ~/.local/bin (add to PATH if necessary)
        meson install -C build
   ```
* Output by default to current working directory.

### Windows

* Can be built from source using Msys2.
    ```
        pacman -Syu
        pacman -S gtk3 vala meson ninja json-glib
        meson build --buildtype=release --strip
		meson install -C build
        ```
### MacOS

* Use Homebrew:
    ```
        # install requirements:
        brew install meson vala gtk+3 json-glib
        # Once (setup)
        meson build --buildtype=release --strip --prefix=~/.local
        # Build and install to ~/.local/bin (add to PATH if necessary)
        meson install -C build
   ```
* If there are missing icons, it may be necessary to `brew install adwaita-icon-theme`.

## Binary Release Media

At some stage, a Debian package and a Windows package (either Zip or an Installer) will be available.

## Miscellaneous

Licence : GPL v3 or later
(c) Jonathan Hudson 1984 - 2022

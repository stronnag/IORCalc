# IORCalc

Calculate IOR ratings like it's 1988 again.

## Introduction

`iorcalc` is an application to input / edit IOR measurement data and generate IOR rating certificates. The application includes all rule amendments to January 1988; after which the author started sailing on non-IOR (CHS) yachts.

`iorcalc` was last validated against official (RORC Rating Office) certificates in 1986; any 1987-1988 rule changes are not validated.

`iorcalc` is believed to be a complete coverage of the IOR rule amended to January 1988; including esoteric rigs (and Appendix 7 (gaff)).

`iorcalc` was written in 1984/5 to run on a Sinclair QL (QDOS OS). It was largely forgotten until the author stumbled upon an old floppy disk, after which it was updated and a modern UI added.

## Usage

### Sample Data

Two sample data files are in the repository `samples` directory.

These are also provided in the binary packages, in `$INSTALLTION_ROOT/share/doc/iorcalc/` (where `$INSTALLTION_ROOT` is the base of the installed package). Using the provided binary packages, `/usr` on Linux and `Program Files\iorcalc` on Windows.

### Data Entry

Launch from a desktop icon or from the command line. If invoked from the command line, data files may be given; drag and drop onto the icon should also work (OS dependent).

Drag and drop onto the [lower status area](#drag-and-drop) is available on all supported OS (FreeBSD, Linux, MacOS and Windows).

```
# Command line, with files
$ iorcalc manitou4.json summerwine.iorbin
```

The main window will be displayed (empty without a data file):

![mainwindow](assets/iorcalc1.png)

The menu (load files etc.) is invoked from the highlighted "hamburger" icon

![menu](assets/iorcalc2.png)

It is possible to open multiple windows (**New Window**) with independent data, calculation and certificate views."Close" closes the window (and quits if it was the only window), "Quit" exits the application.

The [Calculate Rating](#calculate-rating)  option is only enabled when a representative set of data has been input or loaded.

The [Plot Data](#plot-data) option is only enabled when the rating has been calculated and no subsequent changes have been made.

Data may be saved **Save data file**, **Save As data file** at any time.

The **Open data file** option presents a file chooser to allow the selection of previously saved data file (JSON format (or binary)). This will populate the data tabs `Names`, `Hull`, `Afloat/Prop` and `Rig`.

Manual data entry requires that each of the tabs be populated. In particular, the **Calculate Raing** option will not be enabled until the following items are populated:

* Yacht (name)
* Sail number
* Age data
* Hull data: `LOA`, `FGO`, `FD`,  `CMD`,   `MD`, `OMD`, `BMAX`, `B`, `BF`, `BA`, `VHA`, `SBMAX`, `SDM`, `FSS`, `FFS`,  `FFIS`, `FFDS`, `FMDS`, `FAS`, `DMS`.
* Afloat data: `FF`, `FA`, `BWL`, `PL`
* Rig data: `P`, `E`, `BAS` (so cat rigs are OK).

This is just a representative sanity sample, rather than an absolute requirement for calculation.

![menu](assets/iorcalc3.png)

### Calculate Rating

Once sufficient data is entered to calculate the rating, this may be invoked from the main menu.

After the calculation is completed:

* A summary is presented below the input form
* The [Plot Data](#plot-data) menu item and [View Certificate](#viewing-the-certificate) button/menu item are enabled.

It is possible that the data sanity checks are insufficient, and invalid or missing user entered data could cause the application to crash. In this case, your data will be saved as `$TMP/.iorcalc-save.json` (`$TMP` represents the operating system's temporary directory, typically `/tmp` on Linux / FreeBSB).

### Plot Data

The **Plot Data** option opens a new window that shows crude line plots of the yacht's profile, plan and composite BMAX / Mid Depth section:

![plot01](assets/iorcalcp01.png)

![plot02](assets/iorcalcp02.png)

![plot03](assets/iorcalcp03.png)

Note the red, dashed line between `OMD` and `BWL`, to indicate these are from different sections of the yacht which may be separated by a metre or more. Only straight lines are used between points.

### Viewing the Certificate

When the **Show Certificate** is invoked, the certificate will be displayed in a separate scrollable  window.

![certificate](assets/iorcalc4.png)

This also has a menu:

![certificate](assets/iorcalc5.png)

There are three options:

* **Save certificate** : Saves the certificate as a text document. The combo box (bottom left) allows saving as either a single (wide, landscape) page or two separate portrait pages separated by `FORMFEED` (`Ctrl-L`).
* **Print** : Prints the certificate, using the operating system Print Dialog. If `portrait` is selected, the certificate is printed on two sheets, if `landscape`, the certificate is printed on a single sheet. Print to file (PDF, PS, SVG, XPS) as available in the native dialogue. The choices (PDF / Printer, Portrait / Landscape, PDF file name) are cached for future use.

     ![certificate](assets/iorcalc6.png)

* **Set printer font**. By default, the system default `Monospace` font is used. This option presents a Font Button allowing an alternate font to be used. This is also cached for future use
     ![certificate](assets/iorcalc7.png)

    Clicking on the Font Button opens a larger dialog allowing selection of a different font.
     ![certificate](assets/iorcalc8.png)

* **Close** : Closes the certificate window.

#### Printing details

* If `portrait` is selected, the selected font and size are used without further analysis. A 10 point font should fit on an A4 / US Letter page. Most operating systems have a `Preview` option to verify that the font will allow output page coverage.
* If `landscape` is selected, the font is scaled to fit the two pages of the certificate  on the printer page. On A4 (and probably US Letter), this results in a readable c. 7pt font. The font will not be scaled above the user specified size.
* On MacOS, it is necessary to build against GTK4 (master branch, default) to get working printing.

### Drag and Drop

It is possible to drag and drop data files onto the lower status area.

![dnd](assets/iorcalc-dnd.png)

The drop area is shown with a "dotted" border when drag and drop is being used, once the drop data is over that area.

A new window will open showing the dropped data (if valid).

## Bug Reports

If `iorcalc` fails to calculate a rating, or calculates an incorrect rating, please raise an issue on the Github issue tracker. If you have an original IOR certificate for the yacht, a scan of that would help resolve any issues. Please also supply the `iorcalc` JSON data file.

## IOR Rule Book

Unfortunately, there appears to be no official IOR "Red Book" online.
The following unofficial sources have been found:

* [Scan of January 1992 version](http://www.acmenovelties.net/sailing/ior_rule/). Unfortunately this is incomplete and is a rather low quality scan. In particular, some of the more interesting Section 3 pages are missing.
* [Nov.1987 version](https://libgen.is/book/index.php?md5=BE960775D0DA5846E15D84D8282EDA7F) and [mirror](https://3lib.net/md5/BE960775D0DA5846E15D84D8282EDA7F). Appears complete.

## Author, Licence

(c) Jonathan Hudson 1984-2023.

GPL3 or later.

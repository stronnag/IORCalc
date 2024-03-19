## This makefile requires that you have copied the following main build files
## into this directory:
#
# ior.h
# iorext.h
# vapi/ior.vapi
# jsonio.vala

## Set this if elsewhere

prefix ?= $$HOME/.local

LIBDIR ?= $(prefix)/lib
INCDIR ?= $(prefix)/include
SRCDIR ?=.
VAPIDIR = $(prefix)/share/vala/vapi

all: vprint vconv cprint

VALAC=valac
VOPTS=-X -D_GNU_SOURCE -X -Ofast --vapidir=$(VAPIDIR) --includedir=$(INCDIR)/ -X -I$(INCDIR)/
VLOPTS=-X -lm -X -L$(LIBDIR) -X -liorcalc
PKGS=--pkg ior --pkg json-glib-1.0
VCSRC=$(SRCDIR)/jsonio.vala vconv.vala
VPSRC=vprint.vala
CPSRC=cprint.c
CFLAGS=-Ofast -D_GNU_SOURCE -Werror -Wall -I$(INCDIR)
LDFLAGS=-L$(LIBDIR) -liorcalc -lm

vconv: $(SRC)
	$(VALAC) $(VOPTS) $(PKGS) $(VCSRC) $(VLOPTS) -o $@

vprint: $(VRSRC)
	$(VALAC) $(VOPTS) $(PKGS)  $(VPSRC) $(VLOPTS) -o $@

cprint: $(CPSRC)
	$(CC) ${CFLAGS} -o $@ $^ $(LDFLAGS)

clean:
	rm -rf vconv vprint cprint

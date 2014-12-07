AM_CFLAGS = @CFLAGS@
AM_LDFLAGS =
# this lists the binaries to produce, the (non-PHONY, binary) targets in
# # the previous manual Makefile
#
bin_PROGRAMS = fuegonetd
fuegonetd_SOURCES = fuegonetd.c provider.c requester.c
fuegonetd_CFLAGS = -I$(top_srcdir)/include
fuegonetd_LDFLAGS = -lpthread

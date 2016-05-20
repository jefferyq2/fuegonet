AM_CFLAGS = @CFLAGS@
AM_LDFLAGS =
# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
# sim 
bin_PROGRAMS = simprovider simrequester
#
# simprovider
#
simprovider_SOURCES = mc-provider.c
simprovider_CFLAGS = -I$(top_srcdir)/include
#
# simrequester
#
simrequester_SOURCES = mc-requester.c
simrequester_CFLAGS = -I$(top_srcdir)/include
#

AM_CFLAGS = $(VARNISHAPI_CFLAGS) -Wall -Werror
AM_LDFLAGS = $(VARNISHAPI_LIBS) $(VMOD_LDFLAGS)

vmod_LTLIBRARIES = libvmod_hashids.la

libvmod_hashids_la_SOURCES = \
  hashids.c \
	vmod_hashids.c

nodist_libvmod_hashids_la_SOURCES = \
	vcc_hashids_if.c \
	vcc_hashids_if.h

dist_man_MANS = vmod_hashids.3

@BUILD_VMOD_HASHIDS@

AM_TESTS_ENVIRONMENT = \
	PATH="$(VMOD_TEST_PATH)" \
	LD_LIBRARY_PATH="$(VARNISH_LIBRARY_PATH)"
TEST_EXTENSIONS = .vtc
VTC_LOG_COMPILER = varnishtest -v
AM_VTC_LOG_FLAGS = -Dvmod_hashids="$(VMOD_HASHIDS)"

TESTS = @VMOD_TESTS@

EXTRA_DIST = \
	vmod_hashids.vcc \
	$(VMOD_TESTS)

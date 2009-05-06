
AM_CXXFLAGS = -Wall -Wextra \
              -Wctor-dtor-privacy -Woverloaded-virtual -Wchar-subscripts \
              -Wpointer-arith -Wcast-align -Wsign-compare

bin_PROGRAMS = toyfb

toyfb_SOURCES = \
	main.cc \
	main-window.cc \
	main-window.hh

global_defines = \
  -DPREFIX=\""$(prefix)"\" \
  -DSYSCONFDIR=\""$(sysconfdir)"\" \
  -DLIBDIR=\""$(libdir)"\" \
  -DDATADIR=\""$(datadir)"\" \
  -DTOYFB_PKGDATADIR=\""$(pkgdatadir)"\" \
  -DGTKMM_DISABLE_DEPRECATED

AM_CPPFLAGS = -I$(top_builddir) -I$(top_srcdir) $(global_defines) \
               ${TOYFB_CFLAGS}

toyfb_LDADD = ${TOYFB_LIBS}

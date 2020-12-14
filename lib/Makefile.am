backendplugindir = $(pkglibdir)/plugin/backend
backendplugin_LTLIBRARIES = libbackendpsql.la
libbackendpsql_la_SOURCES = backend-psql.c \
	../src/dataframe.c \
	../src/registry.c \
	../src/mem.c \
	../src/assert.c \
	../src/except.c
libbackendpsql_la_CPPFLAGS = -I$(top_srcdir)/include

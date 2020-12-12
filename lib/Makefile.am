backendplugindir = $(pkglibdir)/plugin/backend
backendplugin_LTLIBRARIES = libbackendpsql.la
libbackendpsql_la_SOURCES = backend-psql.c \
	../src/dataframe.c \
	../src/registry.c
libbackendpsql_la_CPPFLAGS = -I$(top_srcdir)/include

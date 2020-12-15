backendplugindir = $(pkglibdir)/plugin/backend
backendplugin_LTLIBRARIES = biblio_backend_psql.la
biblio_backend_psql_la_SOURCES = backend-psql.c \
	../src/dataframe.c \
	../src/registry.c \
	../src/mem.c \
	../src/assert.c \
	../src/except.c \
	../src/common-string.c
biblio_backend_psql_la_CPPFLAGS = -I$(top_srcdir)/include
biblio_backend_psql_la_LDFLAGS = -module

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

frontendplugindir = $(pkglibdir)/plugin/frontend
frontendplugin_LTLIBRARIES = biblio_frontend_console.la
biblio_frontend_console_la_SOURCES = frontend-console.c \
	../src/dataframe.c \
	../src/registry.c \
	../src/mem.c \
	../src/assert.c \
	../src/except.c \
	../src/common-string.c
biblio_frontend_console_la_CPPFLAGS = -I$(top_srcdir)/include
biblio_frontend_console_la_LDFLAGS = -module

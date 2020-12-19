backendplugindir = $(pkglibdir)/plugin/backend
backendplugin_LTLIBRARIES = biblio_backend_psql.la \
	biblio_frontend_console.la \
	biblio_frontend_ncurses.la

biblio_backend_psql_la_SOURCES = backend-psql.c \
	../src/assert.c \
	../src/backend.c \
	../src/common-string.c \
	../src/dataframe.c \
	../src/except.c \
	../src/mem.c \
	../src/registry.c
biblio_backend_psql_la_CPPFLAGS = -I$(top_srcdir)/include
biblio_backend_psql_la_LDFLAGS = -module -ldl -lpq

biblio_frontend_console_la_SOURCES = frontend-console.c \
	../src/article.c \
	../src/assert.c \
	../src/common-string.c \
	../src/dataframe.c \
	../src/except.c \
	../src/frontend.c \
	../src/mem.c \
	../src/registry.c
biblio_frontend_console_la_CPPFLAGS = -I$(top_srcdir)/include
biblio_frontend_console_la_LDFLAGS = -module -ldl -lreadline

biblio_frontend_ncurses_la_SOURCES = frontend-ncurses.c \
	../src/article.c \
	../src/assert.c \
	../src/common-string.c \
	../src/dataframe.c \
	../src/except.c \
	../src/frontend.c \
	../src/mem.c \
	../src/registry.c
biblio_frontend_ncurses_la_CPPFLAGS = -I$(top_srcdir)/include
biblio_frontend_ncurses_la_LDFLAGS = -module -ldl -lncurses -lmenu

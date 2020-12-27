backendplugindir = $(pkglibdir)/plugin/backend
backendplugin_LTLIBRARIES = biblio_backend_psql.la

biblio_backend_psql_la_SOURCES = backend-psql.c
biblio_backend_psql_la_LDFLAGS = -module -ldl -lpq
biblio_backend_psql_la_LIBADD = ../common/libbibliocommon.la

frontendplugindir = $(pkglibdir)/plugin/frontend
frontendplugin_LTLIBRARIES = biblio_frontend_console.la \
	biblio_frontend_ncurses.la \
	biblio_frontend_server.la

biblio_frontend_console_la_SOURCES = frontend-console.c
biblio_frontend_console_la_LDFLAGS = -module -ldl -lreadline
biblio_frontend_console_la_LIBADD = ../common/libbibliocommon.la

biblio_frontend_ncurses_la_SOURCES = frontend-ncurses.c
biblio_frontend_ncurses_la_LDFLAGS = -module -ldl -lncurses -lmenu
biblio_frontend_ncurses_la_LIBADD = ../common/libbibliocommon.la

biblio_frontend_server_la_SOURCES = frontend-server.c
biblio_frontend_server_la_LDFLAGS = -module -ldl -lreadline
biblio_frontend_server_la_LIBADD = ../common/libbibliocommon.la

AM_CPPFLAGS = -I$(top_srcdir)/include


bin_PROGRAMS = biblio
biblio_SOURCES = article.c \
	assert.c \
	backend.c \
	biblio.c \
	common-string.c \
	configparse.c \
	dataframe.c \
	dict.c \
	except.c \
	frontend.c \
	mem.c \
	registry.c \
	frontend-ncurses.c
biblio_CPPFLAGS = -I$(top_srcdir)/include
biblio_LDFLAGS = -ldl -lpq -lncurses

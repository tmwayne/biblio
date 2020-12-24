TESTS = test_article \
	test_dict \
	test_registry \
	test_string

check_PROGRAMS = test_article \
	test_dict \
	test_registry \
	test_string

test_article_SOURCES = test-article.c
test_article_LDADD = ../common/libbibliocommon.la

test_dict_SOURCES = test-dict.c
test_dict_LDADD = ../common/libbibliocommon.la

test_registry_SOURCES = test-registry.c
test_registry_LDADD = ../common/libbibliocommon.la

test_string_SOURCES = test-string.c
test_string_LDADD = ../common/libbibliocommon.la

AM_CPPFLAGS = -I$(top_srcdir)/include

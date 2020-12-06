#!/bin/bash

files="\
aclocal.m4
autom4te.cache
compile
config.h
config.h.in
config.log
config.status
configure
depcomp
install-sh
Makefile
Makefile.in
missing
stamp-h1
src/Makefile
src/Makefile.in
src/.deps
src/*.o
test-driver
tests/Makefile
tests/Makefile.in
tests/*.o
tests/*.log
tests/*.trs
tests/.deps
"

for f in "$files"; do rm -rf $f; done

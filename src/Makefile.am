SUBDIRS = opentmf

lib_LTLIBRARIES = libopentmf.la

include_HEADERS = opentmf.h

libopentmf_la_SOURCES = \
	internal.h \
	core.c

libopentmf_la_CFLAGS = \
	-DOPENTMF_LIBRARY

libopentmf_la_LIBADD = \
	-ldl

libopentmf_la_LDFLAGS = \
	-no-undefined \
	-fvisibility=hidden

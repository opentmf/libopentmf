SUBDIRS = opentmf

lib_LTLIBRARIES = libopentmf.la

include_HEADERS = opentmf.h

libopentmf_la_SOURCES = \
	internal.h \
	core.h \
	core.c \
	driver.h \
	driver.c \
	utils.h

libopentmf_la_CFLAGS = \
	-DOPENTMF_LIBRARY \
	-DOPENTMF_DRIVER_PATH=\"${libdir}/opentmf/drivers\"

libopentmf_la_LIBADD = \
	-ldl

libopentmf_la_LDFLAGS = \
	-no-undefined \
	-fvisibility=hidden

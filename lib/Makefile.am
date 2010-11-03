INCLUDES = -I$(top_srcdir)/include \
	   @POSTGRESQL_CFLAGS@ 
lib_LTLIBRARIES = libazzmos.la
libazzmos_la_SOURCES = utils.c \
		       regexpr.c \
		       uriparse.c \
		       uriobj.c \
		       urinorm.c \
		       httpheaders.c \
		       db.c \
		       uriresolve.c \
		       htmlpage.c
AM_LDFLAGS = @POSTGRESQL_LDFLAGS@ \
	     @LIBCURL@


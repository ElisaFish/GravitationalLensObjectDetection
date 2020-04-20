
#
# Makefile settings
#

CC = gcc

#
# objects and dependencies
#

OBJS = doit.o readfits.o magHist.o
HEAD = readfits.h magHist.h

$(OBJS) : $(HEAD)

#
# make the application
#

LIBFITS  = -L/Users/lisa/source/cfitsio -lcfitsio

doit : $(OBJS) $(HEAD)
	$(CC) -o doit $(OBJS) $(LIBFITS) -lm

#
# specify how to compile
#

.c.o:
	$(CC) -O -c $<


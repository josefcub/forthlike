#####
#
# Linux/x86 Makefile - Change the following options to suit your needs.
#
#####

# Root directory for the other needed directories
ROOT = ..

# Source code location
SRC = ${ROOT}/src

# Destination for 'make install'
DEST=${ROOT}/bin

# Location of the include files for the project
INCPATH = ${ROOT}/include

# Options for the C compiler
CCOPTS = -ansi -I${INCPATH} -O2 -pedantic -Wall

# Our C compiler
CC = /usr/bin/gcc

# For 'make install'
MV= mv -f

#####
#
# You shouldn't need to change anything below this line.
#
#####

IOBJFILES = interp.o i_arith.o i_branch.o i_manip.o i_misc.o i_string.o

SOBJFILES = debug.o list.o parser.o preload.o strutil.o

OBJFILES = ${SOBJFILES} ${IOBJFILES}

TARGETS = fll fstrip

.c.o:
	${CC} ${CCOPTS} -c $*.c

all: ${TARGETS}
	
fll:	${P} ${OBJFILES} fll.o
	${CC} ${CCOPTS} -o fll ${OBJFILES} fll.o

fstrip:	${P} ${SOBJFILES} fstrip.o
	${CC} ${CCOPTS} -o fstrip ${SOBJFILES} fstrip.o

install: all
	${MV} ${TARGETS} ${DEST}

clean:
	rm -f *~ *.o *.obj fll fstrip

cleano:
	rm -f *~ *.o *.obj
        
all_clean: clean
	rm -f ${DEST}/fll ${DEST}/fstrip ${DEST}/fll.exe ${DEST}/fstrip.exe



#####                                                           #####
#                                                                   #
# MS-DOS Makefile - For Borland Turbo C++ MAKE Utility v3.6         #
#                                                                   #
#####                                                           #####

# Root directory for the other needed directories
ROOT = ..

# Source code location
SRC = $(ROOT)\src

# Destination for 'make install'
DEST = $(ROOT)\bin

# Location of the include files for the project
INCPATH = C:\PROGRAMS\TC30\INCLUDE;$(ROOT)\include

# Location of the library files for compilation.
LIBPATH = c:\programs\tc30\lib

# Options for the C compiler
CCOPTS = -I$(INCPATH) -L$(LIBPATH) -G -O -a -d -mt

# Our C compiler
CC = tcc

# For 'make install'
MV= move

#####                                                           #####
#                                                                   #
# In theory, you should not have to change anything below this line #
#                                                                   #
#####                                                           #####

IOBJFILES = $(SRC)\interp.obj $(SRC)\i_arith.obj $(SRC)\i_branch.obj \
            $(SRC)\i_manip.obj $(SRC)\i_misc.obj $(SRC)\i_string.obj

SOBJFILES = $(SRC)\debug.obj $(SRC)\list.obj $(SRC)\parser.obj \
            $(SRC)\preload.obj $(SRC)\strutil.obj

OBJFILES = $(SOBJFILES) $(IOBJFILES)

TARGETS = fll.exe fstrip.exe

.c.obj:
  $(CC) $(CCOPTS) -o$*.obj -c $*.c

all: $(TARGETS)
        
fll.exe:	$(P) $(OBJFILES) $(SRC)\fll.obj
  $(CC) $(CCOPTS) -efll @&&|
    $(OBJFILES) $(SRC)\fll.obj 
| 

fstrip.exe:	$(P) $(SOBJFILES) $(SRC)\fstrip.obj
  $(CC) $(CCOPTS) -efstrip @&&|
    $(SOBJFILES) $(SRC)\fstrip.obj 
| 

install: all
	$(MV) $(TARGETS) $(DEST)

clean:
  @if exist $(SRC)\*.obj del $(SRC)\*.obj
  @if exist $(SRC)\*.o del $(SRC)\*.o

all_clean: clean
  @if exist fll.exe del fll.exe
  @if exist fstrip.exe del fstrip.exe
  @if exist $(SRC)\fll del $(SRC)\fll
  @if exist $(SRC)\fstrip del $(SRC)\fstrip
  @if exist $(SRC)\fll.exe del $(SRC)\fll.exe
  @if exist $(SRC)\fstrip.exe del $(SRC)\fstrip.exe
  @if exist $(DEST)\fll.exe del $(DEST)\fll.exe
  @if exist $(DEST)\fstrip.exe del $(DEST)\fstrip.exe
  @if exist $(DEST)\fll del $(DEST)\fll
  @if exist $(DEST)\fstrip del $(DEST)\fstrip

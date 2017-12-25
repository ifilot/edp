# set compiler and compile options
EXEC = edp
CXX = g++                                # use the GNU C++ compiler
OPTS = -O3 -Wall -g -Wno-write-strings   # use some optimization, report all warnings and enable debugging
CFLAGS = $(OPTS)                         # add compile flags
LDFLAGS = -lcairo -lpcrecpp              # specify link flags here

# set a list of directories
INCDIR =./include
OBJDIR = ./obj
BINDIR = ./bin
SRCDIR = ./src

# set the include folder where the .h files reside
CFLAGS += -I$(INCDIR) -I$(INCDIR_LAMMPS) -I$(SRCDIR)

# add here the source files for the compilation
SOURCES = edp.cpp mathtools.cpp plotter.cpp scalar_field.cpp planeprojector.cpp

# create the obj variable by substituting the extension of the sources
# and adding a path
_OBJ = $(SOURCES:.cpp=.o)
OBJ = $(patsubst %,$(OBJDIR)/%,$(_OBJ))

all: $(BINDIR)/$(EXEC)

$(BINDIR)/$(EXEC): $(OBJ)
	$(CXX) -o $(BINDIR)/$(EXEC) $(OBJ) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c -o $@ $< $(CFLAGS)

test: $(BINDIR)/$(EXEC)
	$(BINDIR)/$(EXEC)

clean:
	rm -vf $(BINDIR)/$(EXEC) $(OBJ)

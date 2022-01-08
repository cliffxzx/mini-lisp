CC    ?= clang
CXX   ?= clang++

EXE = mini_lisp

CDEBUG = -g -Wall

CXXDEBUG = -g -Wall

CSTD = -std=c99
CXXSTD = -std=c++20

CFLAGS = -Wno-register -O0  $(CDEBUG) $(CSTD)
CXXFLAGS = -Wno-register -O0  $(CXXDEBUG) $(CXXSTD)


CPPOBJ = main interpreter
SOBJ =  parser scanner

FILES = $(addsuffix .cpp, $(CPPOBJ))

OBJS  = $(addsuffix .o, $(CPPOBJ))

CLEANLIST =  $(addsuffix .o, $(OBJ)) $(OBJS) \
				 parser.tab.cc parser.tab.hh \
				 location.hh position.hh \
			    stack.hh parser.output parser.o \
				 scanner.o scanner.yy.cc $(EXE)\

.PHONY: all
all: mlisp

mlisp: clean $(FILES)
	$(MAKE) $(SOBJ)
	$(MAKE) $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXE) $(OBJS) parser.o scanner.o $(LIBS)

parser: parser.y
	bison -d -v parser.y --output=parser.tab.cc
	$(CXX) $(CXXFLAGS) -c -o parser.o parser.tab.cc

scanner: scanner.l
	flex --outfile=scanner.yy.cc $<
	$(CXX)  $(CXXFLAGS) -c scanner.yy.cc -o scanner.o

.PHONY: clean
clean:
	rm -rf $(CLEANLIST)


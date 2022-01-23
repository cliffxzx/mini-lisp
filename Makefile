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

FILES = $(addsuffix .cpp, $(CPPOBJ))

OBJS  = $(addsuffix .o, $(CPPOBJ))

CLEANLIST =  $(addsuffix .o, $(OBJ)) $(OBJS) \
							$(EXE)\

.PHONY: all
all: mlisp

mlisp: clean $(FILES)
	$(MAKE) $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXE) $(OBJS) $(LIBS)

.PHONY: clean
clean:
	rm -rf $(CLEANLIST)


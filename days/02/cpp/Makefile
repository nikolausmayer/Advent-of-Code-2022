##
#
# Author: Nikolaus Mayer, 2022
#
##

## Compilers
CXX ?= g++-12

## Compiler flags; extended in 'debug'/'release' rules
CXXFLAGS += -W -Wall -Wextra -Wpedantic -std=c++23 -O3 -I../../../fmt/include -fsanitize=undefined,leak,address

## Linker flags
LDFLAGS = -L../../../fmt -lfmt -fsanitize=undefined,leak,address

## Every *.cpp file is a C++ source file
SOURCES = $(wildcard *.cpp)
## Every *.hpp file is a header file
HEADERS = $(wildcard *.hpp)

## Build a *.o object file for every source file
OBJECTS = $(addsuffix .o, $(basename $(SOURCES)))
## Derive an "app" name for every "app.o" object file
TARGETS = $(basename $(SOURCES))


## Tell the 'make' program that e.g. 'make clean' is not supposed to 
## create a file 'clean'
##
## "Why is it called 'phony'?" -- because it's not a real target. That 
## is, the target name isn't a file that is produced by the commands 
## of that target.
.PHONY: all clean 

## Do not automatically remove "temporary" object files resulting from
## intermediate build steps.
.SECONDARY: $(OBJECTS)

## An empty suffixes rules disables built-in recipes, like "--no-builtin-rules"
.SUFFIXES:


## Default is release build mode
all: $(TARGETS)

## Remove built object files and the main executable
clean:
	$(info ... deleting built object files and executable  ...)
	-rm $(OBJECTS) $(TARGETS)

## Every object file depends on its source. It may also depend on
## potentially all header files, and of course the makefile itself.
%.o: %.cpp Makefile $(HEADERS)
	$(info ... compiling $@ ...)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@


## A main executable depends on its own object file 
## For this rule to work, built-in recipes need to be disabled (see .SUFFIXES)
%: %.o Makefile ../../../fmt/libfmt.a
	$(info ... linking $@ ...)
	$(CXX) $< $(LDFLAGS) -o $@

## Compile {fmt} (https://github.com/fmtlib/) because GCC 12 does not have std::format
../../../fmt/libfmt.a:
	test -d ../../../fmt || wget https://github.com/fmtlib/fmt/releases/download/9.1.0/fmt-9.1.0.zip; unzip fmt-9.1.0.zip; mv fmt-9.1.0 ../../../fmt; rm fmt-9.1.0.zip;
	test -f ../../../fmt/libfmt.a || cd ../../../fmt; cmake .; make -j fmt;


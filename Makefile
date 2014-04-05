#Compiler to use
CC		=g++

#For packaging
#cjvf = .tar.bz2
#czvf = .tar.gz
#cxvf = .tar.xz
#TODO: add auto debian & APK packaging
# I shall add Android as a build target in the future.
TAROPTS		=cjvf
TAR		=tar
DISTEXT		=.bz2

#C++ standard: use C++11 by default for extra
# goodies like nullptr, auto & for(<T> x:y)
CSTD		=gnu++0x

#different CFLAGS for different targets
CFLAGS_DEBUG	=-DDEBUG -g -Wall -O0 -std=$(CSTD)
CFLAGS_RELEASE	=-DDEBUG -O3 -mmmx -msse -std=$(CSTD)

#select your target here
CFLAGS		=$(CFLAGS_DEBUG)
#CFLAGS		=$(CFLAGS_RELEASE)

#linker flags; standard stuff
LDFLAGS		=-lSDL2

#don't touch these lines; they automatically
# glob all C++ files so no editing is needed. At all.
SOURCES		:=$(shell ls *.cpp)
BLOBS		=$(SOURCES:.cpp=.o)

#comment out the REDIRECT line to enable gcc output
#you will need to remove @ at start of lines, too
REDIRECT_CMD	=1>/dev/null
REDIRECT	=$(REDIRECT_CMD)

#output file name (the final executable)
OUTPUT		=a.out

#version
VERSION		=0.0.1

#auto detect OS
#will be one of LINUX DARWIN CYGWIN_NT UWIN FREEBSD etc.
OPSYS		:=$(shell uname | tr '[:lower:]' '[:upper:]' | cut -f1 -d "-")

#get PWD
WD		:=$(shell pwd)

#defines
#eg. you can use { "Starting on "<< OS << endl } or 
#
#	#ifdef LINUX
#		//do linux stuff
#	#elsifdef CYGWIN_NT
#		//do windows stuff
#	#elsedef
#		//do generic fallback stuff
#
# -D$(OPSYS) is provided mainly for ease of porting to a different platform.
DEFINES_ALL	=-DOS=$(OPSYS) -D$(OPSYS)
DEFINES_DEBUG	=$(DEFINES_ALL) -DDEBUG
DEFINES_RELEASE	=$(DEFINES_ALL) -DRELEASE
DEFINES		=$(DEFINES_DEBUG)

all: init $(SOURCES) $(OUTPUT) final

devel: clean all

init:
	@echo "Building $(OUTPUT) for $(OPSYS)..."

$(OUTPUT): $(BLOBS)
	@echo "[Link] $< -> $@"
	$(CC) $(BLOBS) $(LDFLAGS) -o $@ $(REDIRECT)

.cpp.o:
	@echo "[Comp] $< -> $@"
	$(CC) $(CFLAGS) -c $< $(REDIRECT)

clean:
	@echo "Cleaning up..."
	@rm -rf $(BLOBS) $(OUTPUT) dist/*

dist: clean
	@echo "Creating distribution package..."
	@cd dist
	@$(TAR) --exclude dist -$(TAROPTS) \
		dist/$(OUTPUT)-$(VERSION).tar$(DISTEXT) \
		./*	
	@echo "Look in dist/ for your package."
final:
	@echo "Built $(OUTPUT), $(shell wc -c a.out | cut -f1 -d ' ') bytes."
 

#-------------------------------------------------------------------------------
#	AUTOR
#	Aurea Desarrollo Tecnológico.
#	FECHA
#	Notas:
#-------------------------------------------------------------------------------
APPNAME = myApp
#Compiler:
	CC=g++

#Compiler flags
	CFLAGS=-c -g -Wall
	INCLUIDES=
	LIBS=
#	INCLUIDES=$(shell pkg-config --cflags gtk+-2.0)
#	LIBS=$(shell pkg-config --libs gtk+-2.0)
#Directories
	DIRlib= /usr/local/lib	
	incDIR= /usr/local/include
#main function
	mainP= main
#-------------------------------------------------------------------------------
all: Project

Project: mainP.o rovioSpeak.o
	$(CC) -o $(APPNAME) \
	$(LIBS) \
	$(mainP).o \
	rovioSpeak.o \
	-lespeak -lcurl
	
mainP.o: $(mainP).cpp
	$(CC) $(INCLUIDES) $(CFLAGS) \
	$(mainP).cpp
	
rovioSpeak.o: rovioSpeak.cpp
	$(CC) $(INCLUIDES) $(CFLAGS) \
	rovioSpeak.cpp


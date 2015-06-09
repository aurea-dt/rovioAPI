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

	INCLUIDES=$(shell pkg-config --cflags gtk+-2.0 libgnomeui-2.0 gstreamer-0.10 gstreamer-plugins-base-0.10)
	LIBS=$(shell pkg-config --libs gtk+-2.0 libgnomeui-2.0 gstreamer-0.10 gstreamer-plugins-base-0.10)
#Directories
	DIRlib= /usr/local/lib	
	incDIR= /usr/local/include
#main function
	mainP= main
#-------------------------------------------------------------------------------
all: Project

Project: mainP.o rovioStreaming.o
	$(CC) -o $(APPNAME) \
	$(LIBS) \
	$(mainP).o \
	rovioStreaming.o -lgstinterfaces-0.10
	
mainP.o: $(mainP).cpp
	$(CC) $(INCLUIDES) $(CFLAGS) \
	$(mainP).cpp
	
rovioStreaming.o: rovioStreaming.cpp
	$(CC) $(INCLUIDES) $(CFLAGS) \
	rovioStreaming.cpp


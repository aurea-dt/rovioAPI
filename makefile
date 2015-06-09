#-------------------------------------------------------------------------------
#	Mario Chirinos Colunga
#	Aurea Desarrollo Tecnológico.
#	26 Jun 2013
#	Notas:
#-------------------------------------------------------------------------------
APPNAME = GTKapp
#Compiler:
	CC=g++

#Compiler flags
	CFLAGS=-c -g -Wall -O0
	INCLUIDES=$(shell pkg-config --cflags gtk+-2.0 libgnomeui-2.0 gstreamer-0.10 gstreamer-plugins-base-0.10)
	LIBS=$(shell pkg-config --libs gtk+-2.0 libgnomeui-2.0 gstreamer-0.10 gstreamer-plugins-base-0.10)
#Directories
	DIRlib= /usr/local/lib	
	incDIR= /usr/local/include
	rovioDir = $(CURDIR)
#main function
	mainP= main
#-------------------------------------------------------------------------------
all: Project

Project: mainP.o myfilter.o
	$(CC) -export-dynamic -o $(APPNAME) \
	$(LIBS) -L$(rovioDir) \
	$(mainP).o \
	myfilter.o \
	-lgstinterfaces-0.10 -lADT_Rovio -lespeak -lcurl
	
mainP.o: $(mainP).cpp
	$(CC) $(INCLUIDES) $(CFLAGS) \
	$(mainP).cpp
	
myfilter.o: myfilter.cpp
	$(CC) $(INCLUIDES) $(CFLAGS) \
	myfilter.cpp

